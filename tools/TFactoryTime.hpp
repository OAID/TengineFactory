#ifndef TFACTORY_TIME_HPP
#define TFACTORY_TIME_HPP

#if defined(_MSC_VER)
#include <Windows.h>
#undef min
#undef max
#else
#include <sys/time.h>
#endif
#include <iostream>
#include <string>

namespace TFactory {
class TFactoryTime
{
public:
    static double getCurrentTime();

    static void sleepUS(unsigned int secs);
};
}
#endif