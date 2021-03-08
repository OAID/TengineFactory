#ifndef GENERATEANCHOR_HPP
#define GENERATEANCHOR_HPP

#define num_featuremap 3
#define hard_nms 1
#define blending_nms 2 /* mix nms was been proposaled in paper blaze face, aims to minimize the temporal jitter*/

#define clip(x, y) (x < 0 ? 0 : (x > y ? y : x))

#include "Dataset.hpp"
#include <vector>
#include <math.h>
#include <iostream>

namespace TFactory {
typedef struct Anchor
{
    std::vector<std::vector<float>> anchors = {};
    int anchor_num = 0;
} Anchor;

class GenerateAnchor
{
public:
    GenerateAnchor(/* args */);
    Anchor GenerateDetectAnchor(Dataset *data);
    ~GenerateAnchor();
};
}
#endif