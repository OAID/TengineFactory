#include "GenerateAnchor.hpp"

namespace TFactory {
GenerateAnchor::GenerateAnchor(/* args */)
{
}

Anchor GenerateAnchor::GenerateDetectAnchor(Dataset *data)
{
    Anchor result_anchor;
    std::vector<std::vector<float>> featuremap_size;
    std::vector<std::vector<float>> shrinkage_size;
    std::vector<int> w_h_list = { data->width(), data->height() };
    for (auto size : w_h_list)
    {
        std::vector<float> fm_item;
        for (float stride : data->getStrides())
        {
            fm_item.push_back(ceil(size / stride));
        }
        featuremap_size.push_back(fm_item);
        shrinkage_size.push_back(data->getStrides());
    }
    for (std::size_t index = 0; index < num_featuremap; index++)
    {
        std::vector<float> ws_atfer_scales;
        std::vector<float> hs_after_scales;
        if (data->getMinSizes().size() > 0)
        {
            for (std::size_t j = 0; j < data->getMinSizes()[index].size(); j++)
            {
                ws_atfer_scales.push_back(data->getMinSizes()[index][j] / data->width());
                hs_after_scales.push_back(data->getMinSizes()[index][j] / data->height());
            }
        }
        else if (data->getBaseSizes().size() > 0)
        {
            float sk_x = data->getBaseSizes()[index];
            float sk_y = data->getBaseSizes()[index];

            float box_area = sk_x * sk_y;
            std::vector<float> width_scale;
            std::vector<float> height_scale;
            for (std::size_t rat = 0; rat < data->getRatios().size(); rat++)
            {
                width_scale.push_back(round(sqrt(box_area / data->getRatios()[rat])));
                height_scale.push_back(round(width_scale[rat] * data->getRatios()[rat]));
            }

            for (std::size_t x = 0; x < width_scale.size(); x ++)
            {
                std::vector<float> width_tmp_scale;
                std::vector<float> height_tmp_scale;
                for (std::size_t y = 0; y < data->getScales()[index].size(); y++)
                {
                    width_tmp_scale.push_back(width_scale[x] * data->getScales()[index][y]);
                    height_tmp_scale.push_back(height_scale[x] * data->getScales()[index][y]);
                }
                for (std::size_t z = 0; z < width_tmp_scale.size(); z++)
                {
                    ws_atfer_scales.push_back(width_tmp_scale[z] * 1.0f / data->width());
                    hs_after_scales.push_back(height_tmp_scale[z] * 1.0f / data->height());
                }
            }
        }
        else 
        {
            std::cout << "Error Json Data min_sizes and base_size is worry " << std::endl;
        }
        float scale_w = data->width()  / shrinkage_size[0][index];
        float scale_h = data->height() / shrinkage_size[1][index];
        for (std::size_t j = 0; j < featuremap_size[1][index]; j++)
        {
            for (std::size_t i = 0; i < featuremap_size[0][index]; i++)
            {
                float x_center = (i + 0.5f) / scale_w;
                float y_center = (j + 0.5f) / scale_h;
                for (std::size_t k = 0; k < ws_atfer_scales.size(); k++)
                {
                    if (data->getClip())
                    {
                        result_anchor.anchors.push_back(
                            { clip(x_center, 1), clip(y_center, 1), clip(ws_atfer_scales[k], 1), clip(hs_after_scales[k], 1)});
                    }
                    else
                    {
                        result_anchor.anchors.push_back({ x_center, y_center, ws_atfer_scales[k], hs_after_scales[k] });
                    }
                }
            }
        }
    }
    result_anchor.anchor_num = result_anchor.anchors.size();
    return result_anchor;
}

GenerateAnchor::~GenerateAnchor()
{
}
}