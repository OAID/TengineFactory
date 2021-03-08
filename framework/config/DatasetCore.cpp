#include "DatasetCore.hpp"

namespace TFactory {
std::shared_ptr<DatasetCore> DatasetCore::instance = nullptr;

DatasetCore::DatasetCore(/* args */)
{
}

std::shared_ptr<Dataset> DatasetCore::getDataset(const char* FunctionName)
{
    for (std::size_t i = 0; i < functionList.size(); i++)
    {
        if (functionList[i]->getFunctionName().compare(FunctionName) == 0)
        {
            return functionList[i];   
        }
    }
    return nullptr;
}

DatasetCore::~DatasetCore()
{
}
}