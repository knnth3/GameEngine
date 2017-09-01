#include "Time.h"
#include <ctime>


namespace Lime
{
    Time::Time()
    {
        m_Scale = 1.0f;
        start.time.store((float)clock());
        running.time.store(start.time);
    }

    Time::Time(float scale)
    {
        m_Scale = scale;
        start.time.store((float)clock());
        running.time.store(start.time);
    }

    void Time::SetSpeed(float scale)
    {
        m_Scale = scale;
    }

    float Time::GetSpeed()
    {
        return m_Scale;
    }

    void Time::Update()
    {
        running.time.store((float)clock() - start.time);
    }

    float TimeVar::seconds()
    {
        return time / CLOCKS_PER_SEC;
    }

    float TimeVar::milliseconds()
    {
        return time;
    }
}
