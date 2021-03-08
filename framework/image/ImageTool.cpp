#include "ImageTool.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

namespace Tengine {
namespace CV {
namespace ImageTool {

int ErrorCall(int cols, int rows, int channel)
{
    if (cols == 0)
    {
        std::cout << "cols cannot be 0" << std::endl;
        return -1;
    }
    if (rows == 0)
    {
        std::cout << "rows cannot be 0" << std::endl;
        return -2;
    }
    if (channel == 0)
    {
        std::cout << "channel cannot be 0" << std::endl;
        return -3;
    }
    return 0;
}

Mat create(int cols, int rows, int channel)
{
    Mat mat;
    if (ErrorCall(cols, rows, channel) < 0) return mat;
    mat.data = (uint8_t*)malloc(cols * rows * channel);
    mat.cols = cols;
    mat.rows = rows;
    mat.channel = channel;
    return mat;
}

Mat create(int cols, int rows, int channel, uint8_t* data)
{
    Mat mat;
    if (ErrorCall(cols, rows, channel) < 0) return mat;
    mat.data = (uint8_t*)malloc(cols * rows * channel);
    memcpy(mat.data, data, cols * rows * channel);
    mat.cols = cols;
    mat.rows = rows;
    mat.channel = channel;
    return mat;
}

Mat read(char const *filename)
{
    Mat mat;
    std::string path(filename);
    std::string image_type = path.substr(path.length() - 3, path.length() - 1);
    int req_comp = 3;
    if (image_type == "jpg")
    {
        req_comp = 3;
    }
    else if (image_type == "png")
    {
        req_comp = 4;
    }
    else 
    {
        std::cout << "Only support jpg & png Image" << std::endl;
        return mat;
    }
    auto imageData = stbi_load(filename, &mat.cols, &mat.rows, &mat.channel, req_comp);
    mat.data = imageData;
    return mat;
}

int write_jpg(char const *filename, Mat& im)
{
    if (ErrorCall(im.cols, im.rows, im.channel) < 0) return -1;
    stbi_write_jpg(filename, im.cols, im.rows, im.channel, im.data, im.channel * im.cols);
    return 0;
}

int write_png(char const *filename, Mat& im)
{
    if (ErrorCall(im.cols, im.rows, im.channel) < 0) return -1;
    stbi_write_png(filename, im.cols, im.rows, 4, im.data, 4 * im.cols);
    return 0;
}

Mat copyTo(Mat& src)
{
    Mat mat;
    if (ErrorCall(src.cols, src.rows, src.channel) < 0) return mat;
    mat.data = (uint8_t*)malloc( src.cols * src.rows * src.channel);
    memcpy(mat.data, src.data, src.cols * src.rows * src.channel);
    mat.cols = src.cols;
    mat.rows = src.rows;
    mat.channel = src.channel;
    return mat;
}

int copyTo(Mat& src, Mat& dst)
{
    if (ErrorCall(src.cols, src.rows, src.channel) < 0) return -1;
    dst.data = (uint8_t*)malloc( src.cols * src.rows * src.channel);
    memcpy(dst.data, src.data, src.cols * src.rows * src.channel);
    dst.cols = src.cols;
    dst.rows = src.rows;
    dst.channel = src.channel;
    return 0;
}

void free_Mat(Mat &mat)
{
    stbi_image_free(mat.data);
}

}
}
}