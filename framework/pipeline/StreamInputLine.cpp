#include "StreamInputLine.hpp"

namespace TFactory {
StreamInputLine::StreamInputLine(/* args */)
{
}

void StreamInputLine::onCreate()
{
    if (inited) return;
    tengineHandler->createTengineTensor(dataHandler.get());
}

void StreamInputLine::onPreProcess()
{

}

void StreamInputLine::onRun(uint8_t* imageData, int input_w, int input_h, std::vector<std::vector<float*>> streams)
{
    contents = dataHandler->getInputStreamContent();
    std::vector<uint8_t*> mImageInputs;
    for (std::size_t i = 0; i < contents.size(); i++)
    {
        if (contents[i].stream.compare("rect") == 0)
        {
            mBoxes.clear();
            for (std::size_t j = 0; j < streams[i].size(); j++)
            {
                BBox box;
                box.x1 = streams[i][j][1];
                box.y1 = streams[i][j][2];
                box.x2 = streams[i][j][3];
                box.y2 = streams[i][j][4];
                mBoxes.push_back(box);
            }
            mImageInputs = imageHandler->cutOutRect(imageData, input_w, input_h, mBoxes, dataHandler.get());
        }
    }
    mImageOutputs.clear();
    for (std::size_t i = 0; i < mImageInputs.size(); i++)
    {
        tengineHandler->setTensorBuffer<float>(mImageInputs[i], dataHandler.get());
        delete[] mImageInputs[i];
        tengineHandler->runTengine(dataHandler->getFunctionName());
        setImageOutput();
    }
    mImageInputs.clear();
}

void StreamInputLine::setImageOutput()
{
    std::vector<DicString> tensor_names = dataHandler->getOutputTensorString();
    if (tensor_names.size() > 0)
    {
        for (std::size_t i = 0; i < tensor_names.size(); i++)
        {
            float *output = tengineHandler->getTensorOutput(dataHandler->getFunctionName(), tensor_names[i].name.c_str());
            mImageOutputs.push_back(output);
        }
    }
    else
    {
        std::vector<float*> tengine_outputs = tengineHandler->getTensorOutputs(dataHandler->getFunctionName());
        for (std::size_t i = 0; i < tengine_outputs.size(); i++)
        {
            mImageOutputs.push_back(tengine_outputs[i]);
        }
    }
}

void StreamInputLine::onPostProcess()
{
    clearBuffer();
    setStreamPostOutput();
    PostProcessType type = getPostProcessType(dataHandler->getPostProcessMode());
    switch (type)
    {
        case PostProcessType::PostProcessNone:
            outputBuffer = mImageOutputs;
            break;
        default:
            break;
    }
    inited = true;
}

void StreamInputLine::setStreamPostOutput()
{
    std::vector<StreamContent> outputContent = dataHandler->getOutputStreamContent();
    for (std::size_t i = 0; i < contents.size(); i++)
    {
        if (contents[i].stream.compare("rect") == 0)
        {
            int points = 0;
            for (std::size_t k = 0; k < outputContent.size(); k++)
            {
                if (outputContent[k].function.compare("points") == 0)
                {
                    points = std::atoi(outputContent[k].stream.c_str());
                }
            }
            if (points == 0)
            {
                points = tengineHandler->getTensorOutputBufferSize<float>(dataHandler->getFunctionName(), 0);
            }
            for (std::size_t j = 0; j < mImageOutputs.size(); j++)
            {
                float box_w = (mBoxes[j].x2 - mBoxes[j].x1);
                float box_h = (mBoxes[j].y2 - mBoxes[j].y1);

                for (int p = 0; p < points * 2; p = p + 2)
                {
                    mImageOutputs[j][p] = (mImageOutputs[j][p] * box_w * 1.4f - box_w * 0.2f + mBoxes[j].x1);
                    mImageOutputs[j][p + 1] = (mImageOutputs[j][p + 1] * box_h * 1.4f - box_h * 0.2f + mBoxes[j].y1);
                }
            }
        }
    }
}

std::vector<float*> StreamInputLine::onReceiveOutput()
{
    return outputBuffer;
}

void StreamInputLine::onDestory()
{
}

StreamInputLine::~StreamInputLine()
{
    clearBuffer();
}
}