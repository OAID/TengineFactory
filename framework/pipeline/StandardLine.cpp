#include "StandardLine.hpp"
#include "GenerateAnchor.hpp"
#include "WrapExecution.hpp"

namespace TFactory {
StandardLine::StandardLine()
{
}

void StandardLine::onCreate()
{
    if (inited) return;
    tengineHandler->createTengineTensor(dataHandler.get());
}

void StandardLine::onPreProcess()
{
    PreProcessType type = getPreProcessType(dataHandler.get()->getPreProcessMode());
    if (inited) return;
    switch (type)
    {
        case PreProcessType::PreProcessNone:
            break;
        case PreProcessType::CreateAnchor:
            anchor = generateHandler->GenerateDetectAnchor(dataHandler.get());
            break;
        default:
            break;
    }
}

void StandardLine::onRun(uint8_t* imageData, int input_w, int input_h, std::vector<std::vector<float*>> streams)
{
    uint8_t* input = imageHandler->resizeImage(imageData, input_w, input_h, dataHandler.get());
    tengineHandler->setTensorBuffer<float>(input, dataHandler.get());
    delete[] input;
    tengineHandler->runTengine(dataHandler->getFunctionName());
}

void StandardLine::onPostProcess()
{
    PostProcessType type = getPostProcessType(dataHandler->getPostProcessMode());
    switch (type)
    {
        case PostProcessType::PostProcessNone:
            NoPostProcess();
            break;
        case PostProcessType::NMS:
            NMS();
            break;
        default:
            break;
    }
    inited = true;
}

void StandardLine::NoPostProcess()
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

void StandardLine::NMS()
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


std::vector<float*> StandardLine::onReceiveOutput()
{
    return outputBuffer;
}


void StandardLine::onDestory()
{
}

StandardLine::~StandardLine()
{
    clearBuffer();
}
}