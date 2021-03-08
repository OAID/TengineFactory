
#ifndef TengineDefine_h
#define TengineDefine_h

#include <assert.h>
#include <stdio.h>

#if defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#define Tengine_BUILD_FOR_IOS
#endif
#endif

#ifdef Tengine_USE_LOGCAT
#include <android/log.h>
#define Tengine_ERROR(format, ...) __android_log_print(ANDROID_LOG_ERROR, "TengineJNI", format, ##__VA_ARGS__)
#define Tengine_PRINT(format, ...) __android_log_print(ANDROID_LOG_INFO, "TengineJNI", format, ##__VA_ARGS__)
#else
#define Tengine_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#define Tengine_ERROR(format, ...) printf(format, ##__VA_ARGS__)
#endif

#ifdef DEBUG
#define Tengine_ASSERT(x)                                            \
    {                                                            \
        int res = (x);                                           \
        if (!res) {                                              \
            Tengine_ERROR("Error for %s, %d\n", __FILE__, __LINE__); \
            assert(res);                                         \
        }                                                        \
    }
#else
#define Tengine_ASSERT(x)                                        \
    {                                                            \
        int res = (x);                                           \
        if (!res) {                                              \
            Tengine_ERROR("Error for %s, %d\n", __FILE__, __LINE__); \
        }                                                        \
    }
#endif

#define FUNC_PRINT(x) Tengine_PRINT(#x "=%d in %s, %d \n", x, __func__, __LINE__);
#define FUNC_PRINT_ALL(x, type) Tengine_PRINT(#x "=" #type " %" #type " in %s, %d \n", x, __func__, __LINE__);

#if defined(_MSC_VER)
#ifdef BUILDING_DLL
#define Tengine_PUBLIC __declspec(dllexport)
#else
#define Tengine_PUBLIC __declspec(dllimport)
#endif
#else
#define Tengine_PUBLIC __attribute__((visibility("default")))
#endif

#endif /* TengineDefine_h */
