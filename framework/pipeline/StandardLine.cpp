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
            NMS(anchor);
            break;
        case PostProcessType::Split:
            SplitTensorOutput();
            break;
        default:
            break;
    }
    inited = true;
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