#ifndef TENGINE_DATA_HPP
#define TENGINE_DATA_HPP

#include "tengine_c_api.h"
#include <memory>
namespace TFactory {
namespace Tengine {
class TengineData
{
public:
    graph_t graph = nullptr;
    tensor_t input_tensor = nullptr;
    static std::shared_ptr<TengineData> createInstance()
    {
        return std::make_shared<TengineData>();
    }
};


}
}

#endif