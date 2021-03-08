#ifndef ANALYSISCONFIG_HPP
#define ANALYSISCONFIG_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "DatasetCore.hpp"

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace TFactory {
class AnalysisConfig
{
private:
    rapidjson::Document doc;        
    int parseTFactoryConfig();
    int parseModelConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func);
    int parseDetectConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func);
    int parseInputStream(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func);
public:
    AnalysisConfig(/* args */);
    ~AnalysisConfig();
    int parseConfig(const char* jsonPath);
    std::vector<std::shared_ptr<Dataset>> getConfigContent();
};
} // TFactory
#endif