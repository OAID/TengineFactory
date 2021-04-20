#include "LineFunc.hpp"
#include "GenerateAnchor.hpp"

namespace TFactory {
LineFunc::LineFunc(/* args */)
{
}

void LineFunc::NoPostProcess()
{
    std::vector<DicString> tensor_names = dataHandler->getOutputTensorString();
    if (tensor_names.size() > 0)
    {
        for (std::size_t i = 0; i < tensor_names.size(); i++)
        {
            float *output = tengineHandler->getTensorOutput(dataHandler->getFunctionName(), tensor_names[i].name.c_str());
            outputBuffer.push_back(output);
        }
    }
    else
    {
        outputBuffer = tengineHandler->getTensorOutputs(dataHandler->getFunctionName());
    }
}

void LineFunc::NMS(Anchor anchor)
{
    std::vector<DicString> tensor_names = dataHandler->getOutputTensorString();
    float* scores_data;
    float* boxes_data;
    bool all_has_tag = true;
    bool normalize = false;
    clearBuffer();
    for (std::size_t i = 0; i < tensor_names.size(); i++)
    {
        if (tensor_names[i].tag.compare("") == 0)
        {
            all_has_tag = false;
            break;
        }
    }
    if (all_has_tag)
    {
        for (std::size_t i = 0; i < tensor_names.size(); i++)
        {
            if (tensor_names[i].tag.compare("box") == 0)
            {
                boxes_data = tengineHandler->getTensorOutput(dataHandler->getFunctionName(), tensor_names[i].name.c_str());
            }
            else if (tensor_names[i].tag.compare("score") == 0)
            {
                scores_data = tengineHandler->getTensorOutput(dataHandler->getFunctionName(), tensor_names[i].name.c_str());
            }
        }
    }
    else 
    {
        std::vector<float*> outputs = tengineHandler->getTensorOutputs(dataHandler->getFunctionName());
        scores_data = outputs[0];
        boxes_data = outputs[1];
    }
    std::vector<BBox> boxes;
    std::vector<BBox> result;

    backendHandler->GenerateBBox(boxes, anchor, dataHandler.get(), scores_data, boxes_data);
    backendHandler->NMS(boxes, result, dataHandler.get());
    
    if (result.size() > 0)
    {
        if (result[0].x1 > 1 || result[0].x2 > 1 || result[0].y1 > 1 || result[0].y2 > 1)
        {
            normalize = true;
        }
    }
    for (std::size_t i = 0; i < result.size(); i++)
    {
        float* buffer = new float[5];
        buffer[0] = result[i].score;
        if (normalize)
        {
            buffer[1] = result[i].x1 / dataHandler->width();
            buffer[2] = result[i].y1 / dataHandler->height();
            buffer[3] = result[i].x2 / dataHandler->width();
            buffer[4] = result[i].y2 / dataHandler->height();
        }
        else
        {
            buffer[1] = result[i].x1;
            buffer[2] = result[i].y1;
            buffer[3] = result[i].x2;
            buffer[4] = result[i].y2;
        }
        
        outputBuffer.push_back(buffer);
    }
}

void LineFunc::SplitTensorOutput()
{
    std::vector<DicString> tensor_names = dataHandler->getPostProcessString();
    if (tensor_names.size() > 0)
    {
        for (std::size_t i = 0; i < tensor_names.size(); i++)
        {
            float *output = tengineHandler->getTensorOutput(dataHandler->getFunctionName(), tensor_names[i].name.c_str());
            int tensor_output_size = tengineHandler->getTensorOutputBufferSize<float>(dataHandler->getFunctionName(), tensor_names[i].name.c_str());
            if (tensor_names[i].tag.compare("None") != 0)
            {
                int num = std::atoi(tensor_names[i].tag.c_str());
                for (int j = 0; j < tensor_output_size / num; j++)
                {
                    float* per_output = new float[num];
                    for (int k = 0; k < num; k++)
                    {
                        per_output[k] = output[j * num + k];
                    }
                    outputBuffer.push_back(per_output);
                }
            }
            else
            {
                outputBuffer.push_back(output);
            }
        }
    }
    else
    {
        outputBuffer = tengineHandler->getTensorOutputs(dataHandler->getFunctionName());
    }
}

LineFunc::~LineFunc()
{
}
}