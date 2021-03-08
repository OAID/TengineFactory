#include "PipeLine.hpp"
#include "DatasetCore.hpp"
#include "WrapExecution.hpp"

using namespace std;
namespace TFactory {
PipeLine::PipeLine(/* args */)
{
}

void PipeLine::Init()
{
    if (input_handler) return;
    input_handler = std::make_shared<InputHandler>();
    line_manager = std::make_shared<LineData>();
    tengine_controller = std::make_shared<StandardLine>();
    tengine_controller->tengineHandler->initTengine();
    for (std::size_t i = 0; i < DatasetCore::GetInstance()->functionList.size(); i++)
    {
        LineBase *line;
        switch (getPipelineType(DatasetCore::GetInstance()->functionList[i]->getPipelineMode()))
        {
        case PipelineType::Standard:
            line = new StandardLine();
            break;
        case PipelineType::MutiInput:
            line = new MutiInputLine();
            break;
        default:
            break;
        }
        line->dataHandler = DatasetCore::GetInstance()->functionList[i];
        line_manager->PipeLines.push_back(line);
    }
    input_handler->Start(DatasetCore::GetInstance()->sourcesPath);
}

int PipeLine::Run(int index)
{
    if (index >= input_handler->imageCount())
    {
        std::cout << "Tengine Factory Error : Images Index Error" << std::endl;
        return -1;
    }
    input_handler->processData(index);
    uint8_t *input_data = input_handler->buffer();

    RunFactory(input_data, input_handler->width(),  input_handler->height());
    return 0;
}

int PipeLine::RunFactory(uint8_t* input_data, int width, int height)
{
    for (std::size_t i = 0; i < line_manager->PipeLines.size(); i++)
    {
        std::vector<std::vector<float*>> streams;
        std::vector<StreamContent> contents = line_manager->PipeLines[i]->dataHandler->getInputStreamContent();
        for (std::size_t i = 0; i < contents.size(); i++)
        {
            LineBase* line = line_manager->getPipeLine(contents[i].function);
            if (line == nullptr)
            {
                std::cout << "Tengine Factory Error : You must set " << contents[i].function << " function first!" << std::endl;
                return -1;
            }
            else
            {
                streams.push_back(line->outputBuffer);
            }
        }
        line_manager->PipeLines[i]->onCreate();
        line_manager->PipeLines[i]->onPreProcess();
        line_manager->PipeLines[i]->onRun(input_data, width, height, streams);
        line_manager->PipeLines[i]->onPostProcess();
        line_manager->PipeLines[i]->onDestory();
    }
    line_manager->setOutputs();
    return 0;
}

std::vector<FunctionOutputDS> PipeLine::GetOutput()
{
    return line_manager->outputs;
}

void PipeLine::Release()
{
    for (std::size_t i = 0; i < line_manager->PipeLines.size(); i++)
    {
        delete line_manager->PipeLines[i];
    }
    line_manager->PipeLines.clear();
    tengine_controller->tengineHandler->releaseTengine();
    tengine_controller.reset();
}

int PipeLine::GetImageCount()
{
    return input_handler->imageCount();
}

PipeLine::~PipeLine()
{
}
}