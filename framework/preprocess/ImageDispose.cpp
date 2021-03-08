#include "ImageDispose.hpp"
#include <stdio.h>
#include "ImageTool.hpp"

namespace TFactory {
ImageDispose::ImageDispose(/* args */)
{
}

Tengine::CV::ImageFormat getImageFormat(std::string imageFormat)
{
    if (imageFormat == "RGB") {
        return Tengine::CV::ImageFormat::RGB;
    } else if (imageFormat == "BGR") {
        return Tengine::CV::ImageFormat::BGR;
    } else if (imageFormat == "GRAY") {
        return Tengine::CV::ImageFormat::GRAY;
    } else if (imageFormat == "RGBA") {
        return Tengine::CV::ImageFormat::RGBA;
    }
    return Tengine::CV::ImageFormat::RGB;
}

uint8_t* ImageDispose::resizeImage(uint8_t* input, int input_w, int input_h, Dataset* data)
{
    std::shared_ptr<Tengine::CV::ImageProcess> handler = std::shared_ptr<Tengine::CV::ImageProcess>(
        Tengine::CV::ImageProcess::create(Tengine::CV::ImageFormat::RGB, getImageFormat(data->getModelInputType())));

    Tengine::CV::Matrix matrix;

    matrix.postScale(1.0f / input_w, 1.0f / input_h);
    matrix.postScale(data->width(), data->height());

    matrix.invert(&matrix);
    handler->setMatrix(matrix);
    std::size_t buffer_size = data->width() * data->height() * data->channel();
    uint8_t* output = new uint8_t[buffer_size];
    handler->convert(input, input_w, input_h, 0, output, data->width(), data->height());
    return output;
}

std::vector<uint8_t*> ImageDispose::cutOutRect(uint8_t* input, int input_w, int input_h, std::vector<BBox> boxes, Dataset* data)
{
    std::shared_ptr<Tengine::CV::ImageProcess> handler = std::shared_ptr<Tengine::CV::ImageProcess>(
        Tengine::CV::ImageProcess::create(Tengine::CV::ImageFormat::RGB, getImageFormat(data->getModelInputType())));
    std::vector<uint8_t*> output_vec;

    for (std::size_t i = 0; i < boxes.size(); i++){
        float diff_x = (boxes[i].x2 - boxes[i].x1) * 0.2f;
        float diff_y = (boxes[i].y2 - boxes[i].y1) * 0.2f;
        float srcPoints[] = {
            (boxes[i].x1 - diff_x) * input_w, (boxes[i].y1 - diff_y) * input_h,
            (boxes[i].x1 - diff_x) * input_w, (boxes[i].y2 + diff_y) * input_h,
            (boxes[i].x2 + diff_x) * input_w, (boxes[i].y2 + diff_y) * input_h,
            (boxes[i].x2 + diff_x) * input_w, (boxes[i].y1 - diff_y) * input_h
        };

        Tengine::CV::Matrix matrix;
        float dstPoints[] = {
            0.0f, 0.0f,
            0.0f, (float)data->height() - 1,
            (float)data->width() - 1, (float)data->height() - 1,
            (float)data->width() - 1, 0.0f,
        };
        matrix.setPolyToPoly((Tengine::CV::Point *)dstPoints, (Tengine::CV::Point *)srcPoints, 4);
        handler->setMatrix(matrix);
        std::size_t buffer_size = data->width() * data->height() * data->channel();
        uint8_t* output = new uint8_t[buffer_size];
        handler->convert(input, input_w, input_h, 0, output, data->width(), data->height());
#ifdef saveImage
        Tengine::CV::Mat mat;
        mat.cols = data->width();
        mat.rows = data->height();
        mat.channel = 3;
        mat.data = output;
        std::string output_name = "cutoutImage" + std::to_string(i) + ".jpg";
        Tengine::CV::ImageTool::write_jpg(output_name.c_str(), mat);
#endif
        output_vec.push_back(output);
    }
    return output_vec;
}

ImageDispose::~ImageDispose()
{
}
}