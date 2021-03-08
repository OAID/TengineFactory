#include "WrapExecution.hpp"
#include <fstream>
namespace TFactory {
namespace Tengine {
WrapExecution::WrapExecution(/* args */)
{
}

bool IsFileExistent(std::string path) {
    std::ifstream fin(path.c_str());
    if (!fin) return false;
    return true;
}

void WrapExecution::initTengine()
{
    init_tengine();
}

void WrapExecution::createTengineTensor(Dataset* data)
{
    if (!IsFileExistent(data->getModelPath()))
    {
        std::cout << "Tengine Factory Error : Has No Model File" << std::endl;
    }
    if (mTengineMap.count(data->getFunctionName()) != 0)
    {
        std::cout << "Tengine Factory Error : Json File Error " << std::endl;
        return;
    }
    struct options opt;
    opt.num_thread = DatasetCore::GetInstance()->thread;
    opt.cluster = TENGINE_CLUSTER_ALL;
    opt.precision = TENGINE_MODE_FP32;

    std::shared_ptr<TengineData> tengine_data = TengineData::createInstance();
    tengine_data->graph = create_graph(nullptr, "tengine", data->getModelPath().c_str());

    int dims[] = {1, data->channel(), data->height(), data->width() };
    tengine_data->input_tensor = get_graph_input_tensor(tengine_data->graph, 0, 0);
    set_tensor_shape(tengine_data->input_tensor, dims, 4);  

    mTengineMap.insert(std::pair<std::string, std::shared_ptr<TengineData>>(data->getFunctionName(), tengine_data));

    if (prerun_graph_multithread(tengine_data->graph, opt) < 0)
    {
        fprintf(stderr, "Tengine Factory Error : Prerun %s multithread graph failed.\n", data->getFunctionName().c_str());
        return;
    }
}

int WrapExecution::runTengine(std::string functionName)
{
    if (mTengineMap.count(functionName) == 0)
    {
        fprintf(stderr, "Tengine Factory Error : %s runTengine Fail, You must createTengineTensor Fist.\n", functionName.c_str());
        return -1;
    }
    graph_t graph = (mTengineMap.find(functionName)->second)->graph;
#ifdef TFACTORY_TIME
    double start = TFactoryTime::getCurrentTime();
#endif
    int ret = run_graph(graph, 1);
    if (ret < 0)
    {
        std::cout << "Tengine Factory Error : Run Graph Failed !" << std::endl;
    }
#ifdef TFACTORY_TIME
    double end = TFactoryTime::getCurrentTime();
    fprintf(stderr, "Run %s cost %f ms.\n", functionName.c_str(), (end - start));
#endif
    return ret;
}

int WrapExecution::getGraphOutputNodeNum(std::string functionName)
{
    if (mTengineMap.count(functionName) == 0)
    {
        fprintf(stderr, "Tengine Factory Error : %s getGraphOutputNodeNum Fail, You must createTengineTensor Fist.\n", functionName.c_str());
        return -1;
    } 
    graph_t graph = (mTengineMap.find(functionName)->second)->graph;
    return get_graph_output_node_number(graph);
}

float* WrapExecution::getTensorOutput(std::string functionName, int index)
{
    if (mTengineMap.count(functionName) == 0)
    {
        fprintf(stderr, "Tengine Factory Error : %s getTensorOutput Fail, You must createTengineTensor Fist.\n", functionName.c_str());
        return nullptr;
    }
    graph_t graph = (mTengineMap.find(functionName)->second)->graph;
    tensor_t tensor_output = get_graph_output_tensor(graph, index, 0);
    return copyTensorOutputBuffer((float*)get_tensor_buffer(tensor_output), get_tensor_buffer_size(tensor_output));
}

float* WrapExecution::getTensorOutput(std::string functionName, const char* name)
{
    if (mTengineMap.count(functionName) == 0)
    {
        fprintf(stderr, "Tengine Factory Error : %s getTensorOutput Fail, You must createTengineTensor Fist.\n", functionName.c_str());
        return nullptr;
    }
    graph_t graph = (mTengineMap.find(functionName)->second)->graph;
    tensor_t tensor_output = get_graph_tensor(graph, name);
    return copyTensorOutputBuffer((float*)get_tensor_buffer(tensor_output), get_tensor_buffer_size(tensor_output));
}

std::vector<float *> WrapExecution::getTensorOutputs(std::string functionName)
{
    std::vector<float *> result;
    if (mTengineMap.count(functionName) == 0)
    {
        fprintf(stderr, "Tengine Factory Error : %s getTensorOutputs Fail, You must createTengineTensor Fist.\n", functionName.c_str());
        return result;
    }
    graph_t graph = (mTengineMap.find(functionName)->second)->graph;
    int num = get_graph_output_node_number(graph);
    for (int i = 0; i < num; i++)
    {
        tensor_t tensor_output = get_graph_output_tensor(graph, i, 0);
        float* output =  copyTensorOutputBuffer((float*)get_tensor_buffer(tensor_output), get_tensor_buffer_size(tensor_output));
        result.push_back(output);
    }
    return result;
}

float* WrapExecution::copyTensorOutputBuffer(float* tensor_buffer, int buffer_size)
{
    float* output_buffer = new float[buffer_size / sizeof(float)];
    memcpy(output_buffer, tensor_buffer, buffer_size);
    return output_buffer;
}

void WrapExecution::releaseTengine()
{
    for (auto &tengineValue : mTengineMap)
    {
        release_graph_tensor(tengineValue.second->input_tensor);
        postrun_graph(tengineValue.second->graph);
        destroy_graph(tengineValue.second->graph);
        (tengineValue.second).reset();
    }
    mTengineMap.erase(mTengineMap.begin(), mTengineMap.end());
}

WrapExecution::~WrapExecution()
{

}
}
}