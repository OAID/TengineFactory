#ifndef LINEBASE_HPP
#define LINEBASE_HPP

#include <iostream>
#include "GenerateAnchor.hpp"
#include "BackendProcess.hpp"
#include "WrapExecution.hpp"
#include "ImageDispose.hpp"
#include <memory>
#include <stdarg.h>

namespace TFactory {

enum PreProcessType
{
    PreProcessNone,
    CreateAnchor,
};

enum PostProcessType 
{
    PostProcessNone,
    NMS,
};
class LineBase
{
public:
    bool inited = false;
    std::shared_ptr<Dataset> dataHandler;
    std::shared_ptr<Tengine::WrapExecution> tengineHandler = std::make_shared<Tengine::WrapExecution>();;
    std::shared_ptr<ImageDispose> imageHandler = std::make_shared<ImageDispose>();
    std::shared_ptr<GenerateAnchor> generateHandler = std::make_shared<GenerateAnchor>();
    std::shared_ptr<BackendProcess> backendHandler = std::make_shared<BackendProcess>();
    std::vector<float*> outputBuffer;

public:
    PreProcessType getPreProcessType(std::string type)
    {
        if (type.compare("None") == 0) {
            return PreProcessType::PreProcessNone;
        }
        else if (type.compare("CreateAnchor") == 0) {
            return PreProcessType::CreateAnchor;
        }
        return PreProcessType::PreProcessNone;
    }

    PostProcessType getPostProcessType(std::string type)
    {
        if (type.compare("None") == 0) {
            return PostProcessType::PostProcessNone;
        }
        else if (type.compare("NMS") == 0) {
            return PostProcessType::NMS;
        }
        return PostProcessType::PostProcessNone;
    }

    void clearBuffer()
    {
        for (std::size_t i = 0; i < outputBuffer.size(); i++)
        {
            if (outputBuffer[i] != NULL)
            {
                delete[] outputBuffer[i];
            }
        }
        outputBuffer.clear();
    }
public:
    virtual void onCreate() = 0;
    virtual void onPreProcess() = 0;
    virtual void onRun(uint8_t* imageData, int input_w, int input_h, std::vector<std::vector<float*>> streams) = 0;
    virtual void onPostProcess() = 0;
    virtual std::vector<float*> onReceiveOutput() = 0;
    virtual void onDestory() = 0;
    virtual ~LineBase() {};
};
}
#endif