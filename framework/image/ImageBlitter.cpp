
#include "ImageBlitter.hpp"
#include <string.h>
#include <mutex>
#include "Macro.h"
#ifdef Tengine_USE_NEON
#include <arm_neon.h>
#endif

#include <map>
extern "C" {
void NV21ToRGBUnit(const unsigned char* source, unsigned char* dest, size_t countDiv8, const unsigned char* uv);
void NV21ToBGRUnit(const unsigned char* source, unsigned char* dest, size_t countDiv8, const unsigned char* uv);
void NV21ToRGBAUnit(const unsigned char* source, unsigned char* dest, size_t countDiv8, const unsigned char* uv);
}

namespace Tengine {
namespace CV {

static void _gray2C4(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        for (int i = 0; i < countD8; ++i) {
            auto gray = vld1_u8(source + 8 * i);

            uint8x8x4_t rgba;
            rgba.val[0] = gray;
            rgba.val[1] = gray;
            rgba.val[2] = gray;
            rgba.val[3] = vdup_n_u8(255);
            vst4_u8(dest + 32 * i, rgba);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        dest[4 * i + 0] = source[i];
        dest[4 * i + 1] = source[i];
        dest[4 * i + 2] = source[i];
        dest[4 * i + 3] = 255;
    }
}

static void _gray2C3(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        for (int i = 0; i < countD8; ++i) {
            auto gray = vld1_u8(source + 8 * i);

            uint8x8x3_t rgba;
            rgba.val[0] = gray;
            rgba.val[1] = gray;
            rgba.val[2] = gray;
            vst3_u8(dest + 24 * i, rgba);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        dest[3 * i + 0] = source[i];
        dest[3 * i + 1] = source[i];
        dest[3 * i + 2] = source[i];
    }
}

static void _copyC1(const unsigned char* source, unsigned char* dest, size_t count) {
    ::memcpy(dest, source, count * sizeof(unsigned char));
}

static void _copyC4(const unsigned char* source, unsigned char* dest, size_t count) {
    ::memcpy(dest, source, 4 * count * sizeof(unsigned char));
}

static void _copyC3(const unsigned char* source, unsigned char* dest, size_t count) {
    ::memcpy(dest, source, 3 * count * sizeof(unsigned char));
}

static void _rgba2bgra(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        for (int i = 0; i < countD8; ++i) {
            uint8x8x4_t rgba = vld4_u8(source + 32 * i);
            auto t           = rgba.val[0];
            rgba.val[0]      = rgba.val[2];
            rgba.val[2]      = t;
            vst4_u8(dest + 32 * i, rgba);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        dest[4 * i + 0] = source[4 * i + 2];
        dest[4 * i + 1] = source[4 * i + 1];
        dest[4 * i + 2] = source[4 * i + 0];
        dest[4 * i + 3] = source[4 * i + 3];
    }
}
static void _rgba2bgr(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        for (int i = 0; i < countD8; ++i) {
            uint8x8x4_t rgba = vld4_u8(source + 32 * i);

            uint8x8x3_t bgr;
            bgr.val[0] = rgba.val[2];
            bgr.val[1] = rgba.val[1];
            bgr.val[2] = rgba.val[0];
            vst3_u8(dest + 24 * i, bgr);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        dest[3 * i + 0] = source[4 * i + 2];
        dest[3 * i + 1] = source[4 * i + 1];
        dest[3 * i + 2] = source[4 * i + 0];
    }
}
static void _rgb2bgr(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        for (int i = 0; i < countD8; ++i) {
            uint8x8x3_t rgba = vld3_u8(source + 24 * i);
            uint8x8x3_t bgr;
            bgr.val[0] = rgba.val[2];
            bgr.val[1] = rgba.val[1];
            bgr.val[2] = rgba.val[0];
            vst3_u8(dest + 24 * i, bgr);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        dest[3 * i + 0] = source[3 * i + 2];
        dest[3 * i + 1] = source[3 * i + 1];
        dest[3 * i + 2] = source[3 * i + 0];
    }
}
static void _bgra2bgr(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        for (int i = 0; i < countD8; ++i) {
            uint8x8x4_t bgra = vld4_u8(source + 32 * i);

            uint8x8x3_t bgr;
            bgr.val[0] = bgra.val[0];
            bgr.val[1] = bgra.val[1];
            bgr.val[2] = bgra.val[2];
            vst3_u8(dest + 24 * i, bgr);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        dest[3 * i + 0] = source[4 * i + 0];
        dest[3 * i + 1] = source[4 * i + 1];
        dest[3 * i + 2] = source[4 * i + 2];
    }
}

static void _bgra2gray(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        auto rC = vdup_n_u8(19);
        auto gC = vdup_n_u8(38);
        auto bC = vdup_n_u8(7);
        for (int i = 0; i < countD8; ++i) {
            auto rgb   = vld4_u8(source + 32 * i);
            auto res   = vmull_u8(rC, rgb.val[2]) + vmull_u8(gC, rgb.val[1]) + vmull_u8(bC, rgb.val[0]);
            auto resU8 = vshrn_n_u16(res, 6);
            vst1_u8(dest + 8 * i, resU8);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        int r = source[4 * i + 2];
        int g = source[4 * i + 1];
        int b = source[4 * i + 0];

        int y = (19 * r + 38 * g + 7 * b) >> 6;

        dest[i] = y;
    }
}

static void _rgba2gray(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        auto rC = vdup_n_u8(19);
        auto gC = vdup_n_u8(38);
        auto bC = vdup_n_u8(7);
        for (int i = 0; i < countD8; ++i) {
            auto rgb   = vld4_u8(source + 32 * i);
            auto res   = vmull_u8(rC, rgb.val[0]) + vmull_u8(gC, rgb.val[1]) + vmull_u8(bC, rgb.val[2]);
            auto resU8 = vshrn_n_u16(res, 6);
            vst1_u8(dest + 8 * i, resU8);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        int r = source[4 * i + 0];
        int g = source[4 * i + 1];
        int b = source[4 * i + 2];

        int y = (19 * r + 38 * g + 7 * b) >> 6;

        dest[i] = y;
    }
}

static void _rgb2gray(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        auto rC = vdup_n_u8(19);
        auto gC = vdup_n_u8(38);
        auto bC = vdup_n_u8(7);
        for (int i = 0; i < countD8; ++i) {
            auto rgb   = vld3_u8(source + 24 * i);
            auto res   = vmull_u8(rC, rgb.val[0]) + vmull_u8(gC, rgb.val[1]) + vmull_u8(bC, rgb.val[2]);
            auto resU8 = vshrn_n_u16(res, 6);
            vst1_u8(dest + 8 * i, resU8);
        }
        sta = countD8 * 8;
    }
#endif
    for (int i = sta; i < count; ++i) {
        int r = source[3 * i + 0];
        int g = source[3 * i + 1];
        int b = source[3 * i + 2];

        int y = (19 * r + 38 * g + 7 * b) >> 6;

        dest[i] = y;
    }
}

static void _bgr2gray(const unsigned char* source, unsigned char* dest, size_t count) {
    int sta = 0;
#ifdef Tengine_USE_NEON
    int countD8 = (int)count / 8;
    if (countD8 > 0) {
        auto rC = vdup_n_u8(19);
        auto gC = vdup_n_u8(38);
        auto bC = vdup_n_u8(7);
        for (int i = 0; i < countD8; ++i) {
            auto rgb   = vld3_u8(source + 24 * i);
            auto res   = vmull_u8(rC, rgb.val[2]) + vmull_u8(gC, rgb.val[1]) + vmull_u8(bC, rgb.val[0]);
            auto resU8 = vshrn_n_u16(res, 6);
            vst1_u8(dest + 8 * i, resU8);
        }
        sta = countD8 * 8;
    }
#endif

    for (int i = sta; i < count; ++i) {
        int r = source[3 * i + 2];
        int g = source[3 * i + 1];
        int b = source[3 * i + 0];

        int y = (19 * r + 38 * g + 7 * b) >> 6;

        dest[i] = y;
    }
}

void NV21ToRGBA(const unsigned char* source, unsigned char* dest, size_t count) {
    auto y   = source;
    auto uv  = source + count;
    auto dst = dest;
    int sta  = 0;
#ifdef Tengine_USE_NEON
    const int unit   = 16;
    size_t countDiv8 = count / unit;
    if (countDiv8 > 0) {
        NV21ToRGBAUnit(source, dest, countDiv8, uv);
        sta = (int)countDiv8 * unit;
    }
#endif

    for (int i = sta; i < count; ++i) {
        int Y = y[i];
        int U = (int)uv[(i / 2) * 2 + 1] - 128;
        int V = (int)uv[(i / 2) * 2 + 0] - 128;

        Y     = Y << 6;
        int R = (Y + 73 * V) >> 6;
        int G = (Y - 25 * U - 37 * V) >> 6;
        int B = (Y + 130 * U) >> 6;

        R = std::min(std::max(R, 0), 255);
        G = std::min(std::max(G, 0), 255);
        B = std::min(std::max(B, 0), 255);

        dst[4 * i + 0] = (uint8_t)R;
        dst[4 * i + 1] = (uint8_t)G;
        dst[4 * i + 2] = (uint8_t)B;
        dst[4 * i + 3] = 255;
    }
}

void NV21ToRGB(const unsigned char* source, unsigned char* dest, size_t count) {
    auto y   = source;
    auto uv  = source + count;
    auto dst = dest;
    int sta  = 0;
#ifdef Tengine_USE_NEON
    const int unit   = 16;
    size_t countDiv8 = count / unit;
    if (countDiv8 > 0) {
        NV21ToRGBUnit(source, dest, countDiv8, uv);
        sta = (int)countDiv8 * unit;
    }
#endif
    for (int i = sta; i < count; ++i) {
        int Y = y[i];
        int U = (int)uv[(i / 2) * 2 + 1] - 128;
        int V = (int)uv[(i / 2) * 2 + 0] - 128;

        Y     = Y << 6;
        int R = (Y + 73 * V) >> 6;
        int G = (Y - 25 * U - 37 * V) >> 6;
        int B = (Y + 130 * U) >> 6;

        R = std::min(std::max(R, 0), 255);
        G = std::min(std::max(G, 0), 255);
        B = std::min(std::max(B, 0), 255);

        dst[3 * i + 0] = (uint8_t)R;
        dst[3 * i + 1] = (uint8_t)G;
        dst[3 * i + 2] = (uint8_t)B;
    }
}

void NV21ToBGR(const unsigned char* source, unsigned char* dest, size_t count) {
    auto y   = source;
    auto uv  = source + count;
    auto dst = dest;
    int sta  = 0;
#ifdef Tengine_USE_NEON
    const int unit   = 16;
    size_t countDiv8 = count / unit;
    if (countDiv8 > 0) {
        NV21ToBGRUnit(source, dest, countDiv8, uv);
        sta = (int)countDiv8 * unit;
    }
#endif
    for (int i = sta; i < count; ++i) {
        int Y = y[i];
        int U = (int)uv[(i / 2) * 2 + 1] - 128;
        int V = (int)uv[(i / 2) * 2 + 0] - 128;

        Y     = Y << 6;
        int R = (Y + 73 * V) >> 6;
        int G = (Y - 25 * U - 37 * V) >> 6;
        int B = (Y + 130 * U) >> 6;

        R = std::min(std::max(R, 0), 255);
        G = std::min(std::max(G, 0), 255);
        B = std::min(std::max(B, 0), 255);

        dst[3 * i + 0] = (uint8_t)B;
        dst[3 * i + 1] = (uint8_t)G;
        dst[3 * i + 2] = (uint8_t)R;
    }
}

#define CHECKFORMAT(src, dst, func)\
if (source == src && dest == dst) return func

ImageBlitter::BLITTER ImageBlitter::choose(ImageFormat source, ImageFormat dest) {
    if (source == YUV_NV12) {
        source = YUV_NV21;
    }
    CHECKFORMAT(RGBA, RGBA, _copyC4);
    CHECKFORMAT(RGBA, BGRA, _rgba2bgra);
    CHECKFORMAT(RGBA, BGR, _rgba2bgr);
    CHECKFORMAT(RGBA, RGB, _bgra2bgr);
    CHECKFORMAT(RGBA, GRAY, _rgba2gray);
    CHECKFORMAT(BGRA, RGBA, _rgba2bgra);
    CHECKFORMAT(BGRA, BGRA, _copyC4);
    CHECKFORMAT(BGRA, BGR, _bgra2bgr);
    CHECKFORMAT(BGRA, RGB, _rgba2bgr);
    CHECKFORMAT(BGRA, GRAY, _bgra2gray);
    CHECKFORMAT(RGB, RGB, _copyC3);
    CHECKFORMAT(RGB, BGR, _rgb2bgr);
    CHECKFORMAT(RGB, GRAY, _rgb2gray);

    CHECKFORMAT(BGR, BGR, _copyC3);
    CHECKFORMAT(BGR, RGB, _rgb2bgr);
    CHECKFORMAT(BGR, GRAY, _bgr2gray);

    CHECKFORMAT(GRAY, RGBA, _gray2C4);
    CHECKFORMAT(GRAY, BGRA, _gray2C4);
    CHECKFORMAT(GRAY, BGR, _gray2C3);
    CHECKFORMAT(GRAY, RGB, _gray2C3);
    CHECKFORMAT(GRAY, GRAY, _copyC1);

    CHECKFORMAT(YUV_NV21, GRAY, _copyC1);
    CHECKFORMAT(YUV_NV21, RGB, NV21ToRGB);
    CHECKFORMAT(YUV_NV21, BGR, NV21ToBGR);
    CHECKFORMAT(YUV_NV21, RGBA, NV21ToRGBA);

    return nullptr;
}
} // namespace CV
} // namespace Tengine
