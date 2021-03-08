#ifndef BACKEND_PROCESS_HPP
#define BACKEND_PROCESS_HPP

#include <iostream>
#include "GenerateAnchor.hpp"
#include "ds.hpp"
#include <vector>
#include <algorithm>
#include <string.h>

namespace TFactory {
class BackendProcess
{
public:
    BackendProcess(/* args */);
    void GenerateBBox(std::vector<BBox> &bbox, TFactory::Anchor anchor, Dataset *data, float *scores, float *boxes);
    void NMS(std::vector<BBox> &input, std::vector<BBox> &output, Dataset *data, int type = blending_nms);
    ~BackendProcess();
};
}
#endif