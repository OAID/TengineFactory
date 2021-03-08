#ifndef STANDARDLINE_HPP
#define STANDARDLINE_HPP

#include <iostream>
#include "LineBase.hpp"
#include "Dataset.hpp"
#include "ImageDispose.hpp"
#include <memory>

namespace TFactory {
class StandardLine : public LineBase
{
private:
    Anchor anchor;
public:
    StandardLine();
    void onCreate();
    void onPreProcess();
    void onRun(uint8_t* imageData, int input_w, int input_h, std::vector<std::vector<float*>> streams);
    void onPostProcess();
    std::vector<float*> onReceiveOutput();
    void onDestory();
    ~StandardLine();
private:
    void NoPostProcess();
    void NMS();
};
}
#endif