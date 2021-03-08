
#ifndef ImageBlitter_hpp
#define ImageBlitter_hpp

#include <stdio.h>
#include <ImageProcess.hpp>

namespace Tengine {
namespace CV {
class ImageBlitter {
public:
    typedef void (*BLITTER)(const unsigned char* source, unsigned char* dest, size_t count);

    static BLITTER choose(ImageFormat source, ImageFormat dest);
};
} // namespace CV
} // namespace Tengine

#endif /* ImageBlitter_hpp */
