#include "InputHandler.hpp"
#include "TFactoryFiles.hpp"

namespace TFactory {
InputHandler::InputHandler(/* args */)
{
}

void InputHandler::Start(std::string path)
{
    imageFileNames.clear();
    if (path.compare("") == 0 || path.compare("Video") == 0)
    {
        cap = cv::VideoCapture(0);
        if (!cap.isOpened())
            return;
    }
    else
    {
        if (path.find(".jpg") != std::string::npos || path.find(".png") != std::string::npos)
        {
            if (!TFactoryFiles::file_exist(path.c_str()))
            {
                std::cout << "Error : Image File Path is not correct" << std::endl;
                return;
            }
            else
            {
                imageFileNames.push_back(path);
            }
        }
        else 
        {
            imageFileNames = TFactoryFiles::getFilesName(path.c_str());
        }
    }
}

int InputHandler::getVideoData()
{
    cv::Mat frame;
    cap.read(frame);
    w = frame.cols;
    h = frame.rows;
    bpp = frame.channels();
    if (input_data != NULL)
    {
        delete[] input_data;
    }
    input_data = new uint8_t[w * h * bpp];
    memcpy(input_data, frame.data, w * h * bpp);   
    return 0;
}

int InputHandler::getImageData(std::string image_path)
{
    cv::Mat frame = cv::imread(image_path);
    w = frame.cols;
    h = frame.rows;
    bpp = frame.channels();
    if (input_data != NULL)
    {
        delete[] input_data;
    }
    input_data = new uint8_t[w * h * bpp];
    memcpy(input_data, frame.data, w * h * bpp);
    return 0;
}

void InputHandler::processData(int index)
{
    if (index >= 0) {
        getImageData(imageFileNames[index]);
    }
    else {
        getVideoData();
    }
}

InputHandler::~InputHandler()
{
    if (cap.isOpened())
        cap.release();
    if (input_data != NULL)
    {
        delete[] input_data;
    }
}
}