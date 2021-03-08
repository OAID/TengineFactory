#include <iostream>
#include <opencv2/opencv.hpp>
#include "TFactoryProcess.hpp"
#include <memory>

int main(int argc, char **argv)
{
    std::shared_ptr<TFactory::TFactoryProcess> interProcess = std::shared_ptr<TFactory::TFactoryProcess>(
        TFactory::TFactoryProcess::create()
    );
    interProcess->init("./config.json");
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
            if (fCom->functionName.compare("FaceDetect") == 0)
            {
                for (int j = 0; j < fCom->function_output_count; j++)
                {
                    float x1 = fCom->output_buffers[j][1];
                    float y1 = fCom->output_buffers[j][2];
                    float x2 = fCom->output_buffers[j][3];
                    float y2 = fCom->output_buffers[j][4];

                    cv::Point pt1((x1) * image_w, (y1) * image_h);
                    cv::Point pt2((x2) * image_w, (y2) * image_h);
                    cv::rectangle(frame, pt1, pt2, cv::Scalar(255, 0, 0), 2);
                }
            }
            else if (fCom->functionName.compare("FaceLandmark") == 0)
            {
                for (int j = 0; j < fCom->function_output_count; j++)
                {
                    for (int k = 0; k < 424; k = k + 2)
                    {
                        float point_x = fCom->output_buffers[j][k + 0];
                        float point_y = fCom->output_buffers[j][k + 1];
                        cv::Point pt(point_x * image_w, point_y * image_h);
                        cv::circle(frame, pt, 1, cv::Scalar(255, 0, 0), 1);
                    }
                }
            }
        }
        std::string output_name = "face" + std::to_string(index) + ".jpg";
        cv::imwrite(output_name, frame);
    }
    interProcess->release();
    
    return 0;
}