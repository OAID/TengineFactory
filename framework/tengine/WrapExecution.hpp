#ifndef WRAP_EXECUTION_HPP
#define WRAP_EXECUTION_HPP

#include <iostream>
#include "DatasetCore.hpp"
#include "TengineData.hpp"
#include <memory>
#include <map>
#include <cstring>
#include "TFactoryTime.hpp"
#undef setTensorBuffer

namespace TFactory {
namespace Tengine {
class WrapExecution
{
private:
    std::map<std::string, std::shared_ptr<TengineData>> mTengineMap;
    float* copyTensorOutputBuffer(float* tensor_buffer, int buffer_size); 
public:
    WrapExecution();
    void initTengine();
    void createTengineTensor(Dataset* data);
    
    template <typename T> void setTensorBuffer(const uint8_t* source, Dataset *data)
    {
        if (mTengineMap.count(data->getFunctionName()) == 0)
        {
            fprintf(stderr, "Tengine Factory Error : %s setTensorBuffer Fail, You must createTengineTensor Fist.\n", data->getFunctionName().c_str());
            return;
        }
        int buffer_size = data->width() * data->height() * data->channel();
        T* input_buffer = new T[buffer_size];
        int hw = data->width() * data->height();
        for (int h = 0; h < data->height(); h ++)
        {
            for (int w = 0; w < data->width(); w++)
            {
                for (int c = 0 ; c < data->channel(); c++)
                {
                    input_buffer[c * hw + h * data->width() + w] = 
                            ((float)*source - data->getMeanValue()[c]) * data->getNormalValue()[c];
                    source++;
                }
            }
        }
        set_tensor_buffer((mTengineMap.find(data->getFunctionName())->second)->input_tensor, input_buffer, buffer_size * sizeof(T));
    }
    int runTengine(std::string functionName);
    int getGraphOutputNodeNum(std::string functionName);

    float* getTensorOutput(std::string functionName, int index);
    template <typename T> int getTensorOutputBufferSize(std::string functionName, int index)
    {
        if (mTengineMap.count(functionName) == 0)
        {
            fprintf(stderr, "Tengine Factory Error : %s getTensorOutputBufferSize Fail, You must createTengineTensor Fist.\n", functionName.c_str());
            return -1;
        }
        graph_t graph = (mTengineMap.find(functionName)->second)->graph;
        tensor_t tensor_output = get_graph_output_tensor(graph, index, 0);
        return get_tensor_buffer_size(tensor_output) / sizeof(T);
    }

    float* getTensorOutput(std::string functionName, const char* name);
    template <typename T> int getTensorOutputBufferSize(std::string functionName, const char* name)
    {
        if (mTengineMap.count(functionName) == 0)
        {
            fprintf(stderr, "Tengine Factory Error : %s getTensorOutputBufferSize Fail, You must createTengineTensor Fist.\n", functionName.c_str());
            return -1;
        }
        graph_t graph = (mTengineMap.find(functionName)->second)->graph;
        tensor_t tensor_output = get_graph_tensor(graph, name);
        return get_tensor_buffer_size(tensor_output) / sizeof(T);
    }
    
    std::vector<float *> getTensorOutputs(std::string functionName);
    void releaseTengine();
    ~WrapExecution();
};
} // namespace Tengine
} // namespace TFactory
#endif