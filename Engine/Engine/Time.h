#pragma once
#include <atomic>
#include "Primitives.h"

namespace Lime
{
    struct TimeVar
    {
        DLL_EXPORT float seconds();
        DLL_EXPORT float milliseconds();
        std::atomic<float> time;
    };

    class Time
    {
    public:
        DLL_EXPORT Time();
        DLL_EXPORT Time(float scale);

        DLL_EXPORT void SetSpeed(float scale);
        DLL_EXPORT float GetSpeed();
        DLL_EXPORT void Update();

        TimeVar start;
        TimeVar running;


    private:
        std::atomic<float> m_Scale;
    };
}

