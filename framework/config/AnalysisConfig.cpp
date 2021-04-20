#include "AnalysisConfig.hpp"
#include "SplitString.hpp"
#include <fstream>
#include <sstream>

namespace TFactory {

AnalysisConfig::AnalysisConfig(/* args */)
{
}

int AnalysisConfig::parseConfig(const char* jsonPath)
{
    std::ifstream in(jsonPath, std::ios::binary);
 
	if (!in.is_open())
	{
		std::cout << "Tengine Factory Error : cannot open Json File " << jsonPath << std::endl;
		return -1;
	}
	std::string json_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();
    if (!doc.Parse(json_content.c_str()).HasParseError())
    {
        parseTFactoryConfig();
        // 解析算法参数
        for (std::size_t index = 0; index < TFactory::DatasetCore().GetInstance()->functionList.size(); index++)
        {
            std::string funcName = TFactory::DatasetCore().GetInstance()->functionList[index]->getFunctionName();
            if (doc.HasMember(funcName.c_str()) && doc[funcName.c_str()].IsObject())
            {
                const rapidjson::Value& func = doc[funcName.c_str()];
                ErrorLog(parseModelConfig(TFactory::DatasetCore().GetInstance()->functionList[index], func));
                ErrorLog(parsePreprocessConfig(TFactory::DatasetCore().GetInstance()->functionList[index], func));
                ErrorLog(parsePostprecessConfig(TFactory::DatasetCore().GetInstance()->functionList[index], func));
                ErrorLog(paseSupportConfig(TFactory::DatasetCore().GetInstance()->functionList[index], func));
                ErrorLog(parseStreamConfig(TFactory::DatasetCore().GetInstance()->functionList[index], func));
            }
        }
    }
    else
    {
        std::cout << "Tengine Factory Error : parse json config wrong!" << std::endl;
    }
    
    return 0;
}

ConfigErrorCode AnalysisConfig::parseTFactoryConfig()
{
    // 解析功能
    if (doc.HasMember("Function") && doc["Function"].IsArray())
    {
        const rapidjson::Value& funcs = doc["Function"];
        std::size_t func_len = funcs.Size();
        TFactory::DatasetCore().GetInstance()->functionList.clear();
        for (std::size_t i = 0; i < func_len; i++)
        {
            std::shared_ptr<Dataset> data = std::make_shared<Dataset>();
            data->setFunctionName(funcs[i].GetString());
            TFactory::DatasetCore().GetInstance()->functionList.push_back(data);
        }
    }
    // 解析线程数
    if (doc.HasMember("Version") && doc["Version"].IsString())
    {
        TFactory::DatasetCore().GetInstance()->version = doc["Version"].GetString();
    }

    if (doc.HasMember("Sources") && doc["Sources"].IsString())
    {
        TFactory::DatasetCore().GetInstance()->sourcesPath = doc["Sources"].GetString();
    }
    else
    {
        TFactory::DatasetCore().GetInstance()->sourcesPath = "";
    }
    return NO_ERROE;
}

ConfigErrorCode AnalysisConfig::parseModelConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func)
{
    if (func.HasMember("ModelPath") && func["ModelPath"].IsString())
    {
        function_handler->setModelPath(func["ModelPath"].GetString());
    }
    if (func.HasMember("input_w") && func["input_w"].IsInt())
    {
        function_handler->setModelInputWidth(func["input_w"].GetInt());
    }
    if (func.HasMember("input_h") && func["input_h"].IsInt())
    {
        function_handler->setModelInputHeight(func["input_h"].GetInt());
    }
    if (func.HasMember("input_type") && func["input_type"].IsString())
    {
        function_handler->setModelInputType(func["input_type"].GetString());
        if (((std::string)func["input_type"].GetString()).compare("RGB") == 0 || ((std::string)func["input_type"].GetString()).compare("BGR") ==0)
        {
            function_handler->setModelBpp(3);
        }
        else if (((std::string)func["input_type"].GetString()).compare("YUV_NV21") == 0)
        {
            function_handler->setModelBpp(-1);
        }
        else if (((std::string)func["input_type"].GetString()).compare("GRAY") == 0)
        {
            function_handler->setModelBpp(1);
        }
        else if (((std::string)func["input_type"].GetString()).compare("RGBA") == 0 || ((std::string)func["input_type"].GetString()).compare("BGRA") == 0)
        {
            function_handler->setModelBpp(4);
        }
        else 
        {
            return MODEL_IMAGE_FORMAT_ERROR;
        }
    }

    if (func.HasMember("mean") && func["mean"].IsArray())
    {
        const rapidjson::Value& meanArray = func["mean"];
        std::size_t meanLen = meanArray.Size();
        for (std::size_t i = 0; i < meanLen; i++)
        {
            function_handler->setMeanValue(i, meanArray[i].GetFloat());
        }
    }

    if (func.HasMember("normal") && func["normal"].IsArray())
    {
        const rapidjson::Value& normalArray = func["normal"];
        std::size_t normalLen = normalArray.Size();
        for (std::size_t i = 0; i < normalLen; i++)
        {
            function_handler->setNormalValue(i, normalArray[i].GetFloat());
        }
    }

    if (func.HasMember("pipelineMode") && func["pipelineMode"].IsString())
    {
        if (((std::string)(func["pipelineMode"].GetString())).compare("standard") == 0) {
            function_handler->setPipelineMode("Standard");
        }
        else {
            function_handler->setPipelineMode(func["pipelineMode"].GetString());
        }
    }

    return NO_ERROE;
}

ConfigErrorCode AnalysisConfig::parsePreprocessConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func)
{
    if (func.HasMember("preprocess") && func["preprocess"].IsString())
    {
        if (((std::string)(func["preprocess"].GetString())).compare("none") == 0) {
            function_handler->setPreProcessMode("None");
        }
        else {
            std::string preprocessName = func["preprocess"].GetString();
            function_handler->setPreProcessMode(preprocessName);
            if (func.HasMember(preprocessName.c_str()) && func[preprocessName.c_str()].IsObject())
            {
                const rapidjson::Value& preprocessObj = func[preprocessName.c_str()];
                if (preprocessObj.HasMember("min_sizes") && preprocessObj["min_sizes"].IsArray())
                {
                    const rapidjson::Value& minSize = preprocessObj["min_sizes"];
                    std::size_t minLen = minSize.Size();
                    std::vector<std::vector<float>> min_boxes;
                    for (std::size_t i = 0; i < minLen; i++)
                    {
                        std::vector<float> size_data;
                        for (std::size_t j = 0; j < minSize[i].GetArray().Size(); j++)
                        {
                            size_data.push_back(minSize[i].GetArray()[j].GetFloat());
                        }
                        min_boxes.push_back(size_data);
                    }
                    function_handler->setMinSizes(min_boxes);
                } else if (preprocessObj.HasMember("scales") && preprocessObj["scales"].IsArray())
                {
                    const rapidjson::Value& scaleArray = preprocessObj["scales"];
                    std::size_t scaleLen = scaleArray.Size();
                    std::vector<std::vector<float>> scales;
                    for (std::size_t i = 0; i < scaleLen; i++)
                    {
                        std::vector<float> scale_data;
                        for (std::size_t j = 0; j < scaleArray[i].GetArray().Size(); j++)
                        {
                            scale_data.push_back(scaleArray[i].GetArray()[j].GetFloat());
                        }
                        scales.push_back(scale_data);
                    }
                    function_handler->setScales(scales);

                    if (preprocessObj.HasMember("ratios") && preprocessObj["ratios"].IsArray())
                    {
                        const rapidjson::Value& ratiosArray = preprocessObj["ratios"];
                        std::size_t ratiosLen = ratiosArray.Size();
                        std::vector<float> ratios_data;
                        for (std::size_t i = 0; i < ratiosLen; i++)
                        {
                            ratios_data.push_back(ratiosArray[i].GetArray()[i].GetFloat());
                        }
                        function_handler->setRatios(ratios_data);
                    }

                    if (preprocessObj.HasMember("base_sizes") && preprocessObj["base_sizes"].IsArray())
                    {
                        const rapidjson::Value& baseSizesArray = preprocessObj["base_sizes"];
                        std::size_t baseSizeLen = baseSizesArray.Size();
                        std::vector<float> base_sizes_data;
                        for (std::size_t i = 0; i < baseSizeLen; i++)
                        {
                            base_sizes_data.push_back(baseSizesArray[i].GetFloat());
                        }
                        function_handler->setBaseSizes(base_sizes_data);
                    }
                }

                if (preprocessObj.HasMember("strides") && preprocessObj["strides"].IsArray())
                {
                    const rapidjson::Value& strides = preprocessObj["strides"];
                    std::size_t stridesLen = strides.Size();
                    std::vector<float> strides_data;
                    for (std::size_t i = 0; i < stridesLen; i++)
                    {
                        strides_data.push_back(strides[i].GetFloat());
                    }
                    function_handler->setStrides(strides_data);
                }
            }
        }
    }
    return NO_ERROE;
}

ConfigErrorCode AnalysisConfig::parsePostprecessConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func)
{
    if (func.HasMember("postprocess") && func["postprocess"].IsString())
    {
        if (((std::string)(func["postprocess"].GetString())).compare("none") == 0) {
            function_handler->setPostProcessMode("None");
        }
        else {
            std::string postprocessName = func["postprocess"].GetString();
            function_handler->setPostProcessMode(postprocessName);            
            if (func.HasMember(postprocessName.c_str()) && func[postprocessName.c_str()].IsObject())
            {
                const rapidjson::Value& postprocessObj = func[postprocessName.c_str()];
                if (postprocessObj.HasMember("iou_threshold") && postprocessObj["iou_threshold"].IsFloat())
                {
                    function_handler->setIouThreshold(postprocessObj["iou_threshold"].GetFloat());
                }
                if (postprocessObj.HasMember("PostProcessString") && postprocessObj["PostProcessString"].IsArray())
                {
                    const rapidjson::Value& stringArray = postprocessObj["PostProcessString"];
                    std::size_t tensorLen = stringArray.Size();
                    std::vector<DicString> output_tensor;
                    for (std::size_t i = 0; i < tensorLen; i++)
                    {
                        DicString tensor_string;
                        std::string content = stringArray[i].GetString();
                        std::vector<std::string> result = SplitString::strSplit(content, ":");
                        if (result.size() == 1)
                        {
                            tensor_string.name = result[0];
                            tensor_string.tag = "None";
                        }
                        else if (result.size() == 2)
                        {
                            std::cout << result[1] << std::endl;
                            tensor_string.name = result[0];
                            tensor_string.tag = result[1];
                        }
                        output_tensor.push_back(tensor_string);
                    }
                    function_handler->setPostProcessString(output_tensor);
                }
            }
        }
    }
    return NO_ERROE;
}

ConfigErrorCode AnalysisConfig::paseSupportConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func)
{
    if (func.HasMember("TensorOuputString") && func["TensorOuputString"].IsArray())
    {
        const rapidjson::Value& tensorArray = func["TensorOuputString"];
        std::size_t tensorLen = tensorArray.Size();
        std::vector<DicString> output_tensor;
        for (std::size_t i = 0; i < tensorLen; i++)
        {
            DicString tensor_string;
            std::string content = tensorArray[i].GetString();
            std::vector<std::string> result = SplitString::strSplit(content, ":");
            if (result.size() == 1)
            {
                tensor_string.name = result[0];
                tensor_string.tag = "None";
            }
            else if (result.size() == 2)
            {
                tensor_string.name = result[0];
                tensor_string.tag = result[1];
            }
            output_tensor.push_back(tensor_string);
        }
        function_handler->setTensorOutputString(output_tensor);
    }

    if (func.HasMember("clip") && func["clip"].IsBool())
    {
        function_handler->setClip(func["clip"].GetBool());
    }

    if (func.HasMember("score_threshold") && func["score_threshold"].IsFloat())
    {
        function_handler->setScoreThreshold(func["score_threshold"].GetFloat());
    }

    if (func.HasMember("variance") && func["variance"].IsArray())
    {
        const rapidjson::Value& variance = func["variance"];
        std::size_t varianceLen = variance.Size();
        std::vector<float> variance_data;
        for (std::size_t i = 0; i < varianceLen; i++)
        {
            variance_data.push_back(variance[i].GetFloat());
        }  
        function_handler->setVariance(variance_data);
    }
    return NO_ERROE;
}

ConfigErrorCode AnalysisConfig::parseStreamConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func)
{
    if (func.HasMember("input_stream") && func["input_stream"].IsArray())
    {
        const rapidjson::Value& inputStreamArray = func["input_stream"];
        std::size_t inputStreamLen = inputStreamArray.Size();
        std::vector<StreamContent> contents;
        for (std::size_t i = 0; i < inputStreamLen; i++)
        {
            std::string jsonInputStream = inputStreamArray[i].GetString();
            std::vector<std::string> result = SplitString::strSplit(jsonInputStream, ":");
            if (result.size() > 1)
            {
                StreamContent content;
                content.function = result[0];
                content.stream = result[1];
                contents.push_back(content);
            }
        }
        function_handler->setStreamContent(contents, "input");
    }

    if (func.HasMember("output_stream") && func["output_stream"].IsArray())
    {
        const rapidjson::Value& outputStreamArray = func["output_stream"];
        std::size_t outputStreamLen = outputStreamArray.Size();
        std::vector<StreamContent> contents;
        for (std::size_t i = 0; i < outputStreamLen; i++)
        {
            std::string jsonOutputStream = outputStreamArray[i].GetString();
            std::vector<std::string> result = SplitString::strSplit(jsonOutputStream, ":");
            if (result.size() > 1)
            {
                StreamContent content;
                content.function = result[0];
                content.stream = result[1];
                contents.push_back(content);
            }
        }
        function_handler->setStreamContent(contents, "output");
    }
    return NO_ERROE;
}

std::vector<std::shared_ptr<Dataset>> AnalysisConfig::getConfigContent()
{
    return DatasetCore::GetInstance()->functionList;
}

void AnalysisConfig::ErrorLog(ConfigErrorCode code)
{
    if (code != 0)
        std::cout << "Tengine Factory Error : ";
    switch (code)
    {
    case NO_ERROE:
        break;
    case MODEL_IMAGE_FORMAT_ERROR:
         std::cout << "input_type is not a correct image format !" << std::endl;
        break;
    default:
        break;
    }
}

AnalysisConfig::~AnalysisConfig()
{
}
} // TFactory