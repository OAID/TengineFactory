#ifndef LINEDATA_HPP
#define LINEDATA_HPP
#include <vector>
#include "StandardLine.hpp"
#include "StreamInputLine.hpp"

namespace TFactory
{
struct FunctionOutputDS
{
    std::string name;
    std::vector<float*> output;
};

class LineData
{
public:
    std::vector<LineBase*> PipeLines;
    std::vector<FunctionOutputDS> outputs;

    LineBase* getPipeLine(std::string name)
    {
        for (std::size_t i = 0; i < PipeLines.size(); i++)
        {
            if (PipeLines[i]->dataHandler->getFunctionName().compare(name) == 0)
            {
                return PipeLines[i];
            }
        }
        return nullptr;
    }

    void setOutputs()
    {
        outputs.clear();
        for (std::size_t i = 0; i < PipeLines.size(); i++)
        {
            FunctionOutputDS ds;
            ds.name = PipeLines[i]->dataHandler->getFunctionName();
            ds.output = PipeLines[i]->onReceiveOutput();
            outputs.push_back(ds);
        }
    }
};
}

#endif