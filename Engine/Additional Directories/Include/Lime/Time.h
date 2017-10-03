#pragma once
#include <atomic>
#include "Primitives.h"

namespace Lime
{
    struct TimeVar
    {
        AppDLL_API float seconds();
        AppDLL_API float milliseconds();
        std::atomic<float> time;
    };

    class Time
    {
    public:
        AppDLL_API Time();
        AppDLL_API Time(float scale);

        AppDLL_API void SetSpeed(float scale);
        AppDLL_API float GetSpeed();
        AppDLL_API void Update();

        TimeVar start;
        TimeVar running;


    private:
        std::atomic<float> m_Scale;
    };
}

