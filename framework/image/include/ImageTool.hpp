#ifndef IMAGE_TOOLS_H
#define IMAGE_TOOLS_H
#include <iostream>
#include <string>


namespace Tengine {
namespace CV {
struct Mat
{
    uint8_t* data;
    int cols;
    int rows;
    int channel;
};
namespace ImageTool {
    Mat create(int cols, int rows, int channel);
    Mat create(int cols, int rows, int channel, uint8_t* data);

    Mat read(char const *filename);
    int write_jpg(char const *filename, Mat& im);
    int write_png(char const *filename, Mat& im);

    Mat copyTo(Mat& src);
    int copyTo(Mat& src, Mat& dst);

    void free_Mat(Mat &mat);
}
}
}


#endif