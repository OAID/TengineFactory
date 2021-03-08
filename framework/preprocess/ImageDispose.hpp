#ifndef IMAGE_DISPOSE_HPP
#define IMAGE_DISPOSE_HPP

#include <iostream>
#include "ImageProcess.hpp"
#include <memory>
#include <Dataset.hpp>
#include <string>
#include <map>
#include "ds.hpp"

namespace TFactory {

class ImageDispose
{
private:
public:
    ImageDispose();
    uint8_t* resizeImage(uint8_t* input, int input_w, int input_h, Dataset* data);
    std::vector<uint8_t*> cutOutRect(uint8_t* input, int input_w, int input_h, std::vector<BBox> boxes, Dataset* data);
    ~ImageDispose();
};
}
#endif