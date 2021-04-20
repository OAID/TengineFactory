#include <iostream>
#include <opencv2/opencv.hpp>
#include "TFactoryProcess.hpp"
#include <memory>

int main(int argc, char **argv)
{

    const char *class_names[] = {"background", "aeroplane", "bicycle", "bird", "boat", "bottle",
                                 "bus", "car", "cat", "chair", "cow", "diningtable",
                                 "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
                                 "sofa", "train", "tvmonitor"};

    std::shared_ptr<TFactory::TFactoryProcess> interProcess = std::shared_ptr<TFactory::TFactoryProcess>(
        TFactory::TFactoryProcess::create());
    interProcess->init("./config.json", 2);
    int count = interProcess->imageCount();
    std::cout << "Image Count : " << count << std::endl;

    for (int index = 0; index < count; index++)
    {
        interProcess->run(index);
        TFactory::TFactoryComponent *com = interProcess->getComponents();
        int image_w = com->width();
        int image_h = com->height();
        int channel = com->channel();
        uint8_t *input_data = com->buffer();
        cv::Mat frame(image_h, image_w, CV_8UC3, input_data);

        for (std::size_t i = 0; i < com->getComponentsOutput().size(); i++)
        {
            TFactory::TFactoryComponent::FunctionComponent *fCom = com->getComponentsOutput()[i];
            if (fCom->functionName.compare("MobileNetSSD") == 0)
            {
                for (int j = 0; j < fCom->function_output_count; j++)
                {
                    if (fCom->output_buffers[j][1] >= 0.5f)
                    {
                        int class_index = fCom->output_buffers[j][0];
                        float x1 = fCom->output_buffers[j][2];
                        float y1 = fCom->output_buffers[j][3];
                        float x2 = fCom->output_buffers[j][4];
                        float y2 = fCom->output_buffers[j][5];

                        cv::Point pt1((x1)*image_w, (y1)*image_h);
                        cv::Point pt2((x2)*image_w, (y2)*image_h);
                        cv::rectangle(frame, pt1, pt2, cv::Scalar(255, 0, 0), 2);
                        std::cout << class_names[class_index] << std::endl;
                    }
                }
            }
        }
        std::string output_name = "ssd_result.jpg";
        cv::imwrite(output_name, frame);
    }
    interProcess->release();

    return 0;
}