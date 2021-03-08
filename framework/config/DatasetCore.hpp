#ifndef DATASETCORE_HPP
#define DATASETCORE_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Dataset.hpp"
#include <memory>

namespace TFactory {

class DatasetCore
{
public:
    DatasetCore(/* args */);
    ~DatasetCore();
    static std::shared_ptr<DatasetCore> GetInstance()
    {
        if (instance == NULL)
        {
            instance = std::make_shared<DatasetCore>();
        }
        return instance;
    }
    std::shared_ptr<Dataset> getDataset(const char* FunctionName);
public:
    std::vector<std::shared_ptr<Dataset>> functionList;
    int thread  = 1;
    std::string sourcesPath = "";
private:
    static std::shared_ptr<DatasetCore> instance;
};

} //TFactory
#endif