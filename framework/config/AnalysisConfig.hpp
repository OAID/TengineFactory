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
enum ConfigErrorCode
{
    NO_ERROE = 0,

    MODEL_IMAGE_FORMAT_ERROR = 10,

};
class AnalysisConfig
{

private:
    rapidjson::Document doc;        
    ConfigErrorCode parseTFactoryConfig();
    ConfigErrorCode parseModelConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func);
    ConfigErrorCode parsePreprocessConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func);
    ConfigErrorCode parsePostprecessConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func);
    ConfigErrorCode paseSupportConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func);
    ConfigErrorCode parseStreamConfig(std::shared_ptr<Dataset> function_handler, const rapidjson::Value& func);

    void ErrorLog(ConfigErrorCode code);

public:
    AnalysisConfig(/* args */);
    ~AnalysisConfig();
    int parseConfig(const char* jsonPath);
    std::vector<std::shared_ptr<Dataset>> getConfigContent();
};
} // TFactory
#endif