#ifndef MUTIINPUTLINE_HPP
#define MUTIINPUTLINE_HPP

#include <iostream>
#include "LineFunc.hpp"
#include "Dataset.hpp"
#include "ImageDispose.hpp"
#include <memory>

namespace TFactory {
class StreamInputLine : public LineFunc
{
private:
    std::vector<float*> mImageOutputs;
    std::vector<StreamContent> contents;
    std::vector<BBox> mBoxes;

    void setImageOutput();
    void setStreamPostOutput();
public:
    StreamInputLine();
    void onCreate();
    void onPreProcess();
    void onRun(uint8_t* imageData, int input_w, int input_h, std::vector<std::vector<float*>> streams);
    void onPostProcess();
    std::vector<float*> onReceiveOutput();
    void onDestory();
    ~StreamInputLine();
};
}

#endif