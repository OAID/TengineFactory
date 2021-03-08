#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

namespace TFactory {
class InputHandler
{
private:
    int w;
    int h;
    int bpp;
    uint8_t* input_data = NULL;

    /** images */
    std::vector<std::string> imageFileNames;
    /** images */

    /** video */
    cv::VideoCapture cap;
    /** video */

    int getVideoData();
    int getImageData(std::string image_path);
public:
    InputHandler(/* args */);
    void Start(std::string path = "");
    void processData(int index = -1);
    inline int width() const {
        return w;
    }
    inline int height() const {
        return h;
    }
    inline int channel() const {
        return bpp;
    }
    inline int imageCount() const {
        return imageFileNames.size();
    }
    inline uint8_t* buffer() const {
        return input_data;
    }
    ~InputHandler();
};
}
#endif