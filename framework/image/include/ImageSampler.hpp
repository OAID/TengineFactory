
#ifndef ImageSampler_hpp
#define ImageSampler_hpp
#include <ImageProcess.hpp>
namespace Tengine {
namespace CV {
class ImageSampler {
public:
    typedef void (*PROC)(const unsigned char* source, unsigned char* dest, Point* points, size_t sta, size_t count,
                         size_t capacity, size_t iw, size_t ih, size_t yStride);

    static PROC choose(ImageFormat format, Filter type, bool identity);
};
} // namespace CV
} // namespace Tengine
#endif /* ImageSampler_hpp */
