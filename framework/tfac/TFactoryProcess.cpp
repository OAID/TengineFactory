#ifndef TFACTORYPROCESS_CPP
#define TFACTORYPROCESS_CPP

#include "TFactoryProcess.hpp"
#include <memory>
#include "PipeLine.hpp"
#include "AnalysisConfig.hpp"

namespace TFactory {
std::shared_ptr<PipeLine> handler;
std::shared_ptr<AnalysisConfig> analysis_handler;
std::shared_ptr<TFactoryComponent> mCom;

    
TFactoryProcess* TFactoryProcess::create()
{
    return new TFactoryProcess();
}


void TFactoryProcess::init(const char* jsonPath)
{
    if (!handler) handler = std::make_shared<PipeLine>();
    if (!analysis_handler) analysis_handler = std::make_shared<AnalysisConfig>();
    if (!mCom) mCom = std::make_shared<TFactoryComponent>();
    analysis_handler->parseConfig(jsonPath);
    handler->Init();
}

void TFactoryProcess::run(int index)
{
    handler->Run(index);
    mCom->setImageComponent(handler->input()->buffer(), 
                            handler->input()->width(), 
                            handler->input()->height(), 
                            handler->input()->channel());
}

void TFactoryProcess::runWithData(uint8_t* input_data, int width, int height)
{
    handler->RunFactory(input_data, width, height);
    mCom->setImageComponent(input_data, width, height, handler->input()->channel());
}

TFactoryComponent* TFactoryProcess::getComponents()
{
    mCom->clearCached();
    std::vector<FunctionOutputDS> outputs = handler->GetOutput();
    for (std::size_t i = 0; i < outputs.size(); i++)
    {
        struct TFactoryComponent::FunctionComponent *com = new TFactoryComponent::FunctionComponent();
        com->functionName = outputs[i].name;
        com->function_output_count = outputs[i].output.size();
        com->output_buffers = outputs[i].output;
        mCom->setFunctionComponent(com);
    }
    
    return mCom.get();
}

void TFactoryProcess::release()
{
    handler->Release();
    if (handler)
        handler.reset();
    if (analysis_handler)
        analysis_handler.reset();
    if (mCom)
        mCom.reset();
}

int TFactoryProcess::imageCount()
{
    return handler->GetImageCount();
}

}
#endif