
#ifndef ImageFloatBlitter_hpp
#define ImageFloatBlitter_hpp

#include <stdio.h>
#include <ImageProcess.hpp>
namespace Tengine {
namespace CV {
class ImageFloatBlitter {
public:
    typedef void (*BLIT_FLOAT)(const unsigned char* source, float* dest, const float* mean, const float* normal,
                               size_t count);
    // If 4 == dstBpp, use RGBA blit, otherwise use the same as format
    static BLIT_FLOAT choose(ImageFormat format, int dstBpp = 0);
};
} // namespace CV
} // namespace Tengine

#endif /* ImageFloatBlitter_hpp */
