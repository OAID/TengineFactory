#ifndef DATASET_HPP
#define DATASET_HPP

#include <string>
#include <vector>

struct StreamContent
{
    std::string function;
    std::string stream;
};

struct DicString
{
    std::string name;
    std::string tag = "";
};

class Dataset
{
private:
    std::string function_name = "TFactory";
    std::string model_path = "";
    int model_input_w = 0;
    int model_input_h = 0;
    int model_bpp = 0;
    std::string model_input_type = "RGB";
    float mean[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float normal[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    // DetectParam
    std::vector<std::vector<float>> min_sizes;
    std::vector<float> strides;
    std::vector<float> variance;
    float score_threshold = 0;
    float iou_threshold = 0;

    std::vector<std::vector<float>> scales;
    std::vector<float> base_sizes;
    std::vector<float> ratios;
    bool clip = false;

    std::string preprocessMode = "None";
    std::string postprocessMode = "None";
    std::string pipelineMode = "Standard";
    std::vector<DicString> tensor_output_string;
    std::vector<StreamContent> input_stream;
    std::vector<StreamContent> output_stream;
public:
    inline std::string getFunctionName() const {
        return function_name;
    }
    inline std::string getModelPath() const {
        return model_path;
    }
    inline int width() const {
        return model_input_w;
    }
    inline int height() const {
        return model_input_h;
    }
    inline int channel() const {
        return model_bpp;
    }
    inline std::string getModelInputType() const {
        return model_input_type;
    }
    inline float* getMeanValue() const {
        return (float*)mean;
    }
    inline float* getNormalValue() const {
        return (float*)normal;
    }
    inline std::vector<std::vector<float>> getScales() const {
        return scales;
    }
    inline std::vector<std::vector<float>> getMinSizes() const {
        return min_sizes;
    }
    inline std::vector<float> getRatios() const {
        return ratios;
    }
    inline std::vector<float> getStrides() const {
        return strides;
    }
    inline std::vector<float> getVariance() const {
        return variance;
    }
    inline float getScoreThreshold() const {
        return score_threshold;
    }
    inline float getIOUThreshold() const {
        return iou_threshold;
    }
    inline bool getClip() const {
        return clip;
    }
    inline std::vector<float> getBaseSizes() const {
        return base_sizes;
    }
    inline std::vector<StreamContent> getInputStreamContent() const {
        return input_stream;
    }
    inline std::vector<StreamContent> getOutputStreamContent() const {
        return output_stream;
    }
    inline std::string getPreProcessMode() const {
        return preprocessMode;
    }
    inline std::string getPostProcessMode() const {
        return postprocessMode;
    }
    inline std::string getPipelineMode() const {
        return pipelineMode;
    }
    inline std::vector<DicString> getOutputTensorString() const {
        return tensor_output_string;
    }
    
    void setFunctionName(std::string func_name);
    void setModelPath(std::string model_path);
    void setModelInputWidth(int width);
    void setModelInputHeight(int height);
    void setModelBpp(int bpp);
    void setModelInputType(std::string model_type);
    void setMeanValue(int index, float mean);
    void setNormalValue(int index, float normal);
    void setMinSizes(std::vector<std::vector<float>> min_sizes);
    void setScales(std::vector<std::vector<float>> scales);
    void setRatios(std::vector<float> ratio);
    void setStrides(std::vector<float> strides);
    void setVariance(std::vector<float> variance);
    void setScoreThreshold(float scoreThreshold);
    void setIouThreshold(float iouThreshold);
    void setClip(bool clip);
    void setBaseSizes(std::vector<float> base_sizes);
    void setStreamContent(std::vector<StreamContent> content, std::string io);
    void setPreProcessMode(std::string mode);
    void setPostProcessMode(std::string mode);
    void setPipelineMode(std::string mode);
    void setTensorOutputString(std::vector<DicString> ts);
};
#endif