#ifndef DS_HPP
#define DS_HPP

#define num_featuremap 3
#define hard_nms 1
#define blending_nms 2

typedef struct BBox {
    float score;
    float x1;
    float x2;
    float y1;
    float y2;
} BBox;
#endif