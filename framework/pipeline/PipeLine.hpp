#ifndef PIPELINE_HPP
#define PIPELINE_HPP
#include "LineData.hpp"
#include "InputHandler.hpp"
#include <memory>
#include <iostream>

namespace TFactory {
enum PipelineType
{
    Standard,
    MutiInput,
};

class PipeLine
{
private:
    std::shared_ptr<InputHandler> input_handler;
    std::shared_ptr<LineData> line_manager;
    std::shared_ptr<LineBase> tengine_controller;

    PipelineType getPipelineType(std::string type)
    {
        if (type.compare("Standard") == 0) {
            return PipelineType::Standard;
        }
        else if (type.compare("MutiInput") == 0){
            return PipelineType::MutiInput;
        }
        return PipelineType::Standard;
    }

public:
    PipeLine();
    ~PipeLine();

    void Init();
    int Run(int index = -1);
    int RunFactory(uint8_t* input_data, int width, int height);

    std::vector<FunctionOutputDS> GetOutput();
    void Release();
    int GetImageCount();


public:
    inline InputHandler* input() const {
        return input_handler.get();
    }
private:

};
}
#endif
