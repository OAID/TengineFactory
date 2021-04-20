#include "Dataset.hpp"

void Dataset::setFunctionName(std::string func_name) {
    function_name = func_name;
}

void Dataset::setModelPath(std::string _model_path) {
    model_path = _model_path;
}

void Dataset::setModelInputWidth(int width) {
    model_input_w = width;
}

void Dataset::setModelInputHeight(int height) {
    model_input_h = height;
}

void Dataset::setModelBpp(int bpp) {
    model_bpp = bpp;
}

void Dataset::setModelInputType(std::string model_type) {
    model_input_type = model_type;
}

void Dataset::setMeanValue(int index, float _mean) {
    mean[index] = _mean;
}

void Dataset::setNormalValue(int index, float _normal) {
    normal[index] = _normal;
}

void Dataset::setMinSizes(std::vector<std::vector<float>> _min_sizes) {
    min_sizes.clear();
    min_sizes = _min_sizes;
}

void Dataset::setScales(std::vector<std::vector<float>> _scales) {
    scales = _scales;
}

void Dataset::setRatios(std::vector<float> _ratio) {
    ratios = _ratio;
}

void Dataset::setStrides(std::vector<float> _strides) {
    strides.clear();
    strides = _strides;
}

void Dataset::setVariance(std::vector<float> _variance) {
    variance.clear();
    variance = _variance;
}

void Dataset::setScoreThreshold(float scoreThreshold) {
    score_threshold = scoreThreshold;
}

void Dataset::setIouThreshold(float iouThreshold) {
    iou_threshold = iouThreshold;
}

void Dataset::setClip(bool _clip) {
    clip = _clip;
}

void Dataset::setBaseSizes(std::vector<float> _base_sizes) {
    base_sizes.clear();
    base_sizes = _base_sizes;
}

void Dataset::setStreamContent(std::vector<StreamContent> content, std::string io) {
    if (io.compare("input") == 0)
    {
        input_stream.clear();
        input_stream = content;
    }
    else
    {
        output_stream.clear();
        output_stream = content;
    }

}

void Dataset::setPreProcessMode(std::string mode) {
    preprocessMode = mode;
}

void Dataset::setPostProcessMode(std::string mode) {
    postprocessMode = mode;
}

void Dataset::setPipelineMode(std::string mode) {
    pipelineMode = mode;
}

void Dataset::setTensorOutputString(std::vector<DicString> ts) {
    tensor_output_string.clear();
    tensor_output_string = ts;
}

void Dataset::setPostProcessString(std::vector<DicString> ts) {
    postprocess_string.clear();
    postprocess_string = ts;
}

