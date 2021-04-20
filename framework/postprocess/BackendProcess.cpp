#include "BackendProcess.hpp"
namespace TFactory {
BackendProcess::BackendProcess(/* args */)
{
}

void BackendProcess::GenerateBBox(std::vector<BBox> &bbox, TFactory::Anchor anchor, Dataset *data, float *scores, float *boxes)
{
    for (int i = 0; i < anchor.anchor_num; i++) {
        if (scores[i * 2 + 1] > data->getScoreThreshold()) {
            BBox rect;
            float x_center =
                    boxes[i * 4] * data->getVariance()[0] * anchor.anchors[i][2] + anchor.anchors[i][0];
            float y_center =
                    boxes[i * 4 + 1] * data->getVariance()[0] * anchor.anchors[i][3] + anchor.anchors[i][1];
            float w = exp(boxes[i * 4 + 2] * data->getVariance()[1]) * anchor.anchors[i][2];
            float h = exp(boxes[i * 4 + 3] * data->getVariance()[1]) * anchor.anchors[i][3];

            rect.x1 = clip(x_center - w / 2.0, 1) * data->width();
            rect.y1 = clip(y_center - h / 2.0, 1) * data->height();
            rect.x2 = clip(x_center + w / 2.0, 1) * data->width();
            rect.y2 = clip(y_center + h / 2.0, 1) * data->height();
            rect.score = clip(scores[i * 2 + 1], 1);
            bbox.push_back(rect);
        }
    }
}

void BackendProcess::NMS(std::vector<BBox> &input, std::vector<BBox> &output, Dataset *data, int type)
{
    std::sort(input.begin(), input.end(), [](const BBox &a, const BBox &b) { return a.score > b.score; });

    int box_num = input.size();

    std::vector<int> merged(box_num, 0);

    for (int i = 0; i < box_num; i++) {
        if (merged[i])
            continue;
        std::vector<BBox> buf;

        buf.push_back(input[i]);
        merged[i] = 1;

        float h0 = input[i].y2 - input[i].y1 + 1;
        float w0 = input[i].x2 - input[i].x1 + 1;

        float area0 = h0 * w0;

        for (int j = i + 1; j < box_num; j++) {
            if (merged[j])
                continue;

            float inner_x0 = input[i].x1 > input[j].x1 ? input[i].x1 : input[j].x1;
            float inner_y0 = input[i].y1 > input[j].y1 ? input[i].y1 : input[j].y1;

            float inner_x1 = input[i].x2 < input[j].x2 ? input[i].x2 : input[j].x2;
            float inner_y1 = input[i].y2 < input[j].y2 ? input[i].y2 : input[j].y2;

            float inner_h = inner_y1 - inner_y0 + 1;
            float inner_w = inner_x1 - inner_x0 + 1;

            if (inner_h <= 0 || inner_w <= 0)
                continue;

            float inner_area = inner_h * inner_w;

            float h1 = input[j].y2 - input[j].y1 + 1;
            float w1 = input[j].x2 - input[j].x1 + 1;

            float area1 = h1 * w1;

            float score;

            score = inner_area / (area0 + area1 - inner_area);

            if (score > data->getIOUThreshold()) {
                merged[j] = 1;
                buf.push_back(input[j]);
            }
        }
        switch (type) {
            case hard_nms: {
                output.push_back(buf[0]);
                break;
            }
            case blending_nms: {
                float total = 0;
                for (std::size_t i = 0; i < buf.size(); i++) {
                    total += exp(buf[i].score);
                }
                BBox rects;
                memset(&rects, 0, sizeof(rects));
                for (std::size_t i = 0; i < buf.size(); i++) {
                    float rate = exp(buf[i].score) / total;
                    rects.x1 += buf[i].x1 * rate;
                    rects.y1 += buf[i].y1 * rate;
                    rects.x2 += buf[i].x2 * rate;
                    rects.y2 += buf[i].y2 * rate;
                    rects.score += buf[i].score * rate;
                }
                output.push_back(rects);
                break;
            }
            default: {
                printf("wrong type of nms.");
                exit(-1);
            }
        }
    }
}

BackendProcess::~BackendProcess()
{
}
}