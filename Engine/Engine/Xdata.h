#pragma once
#include "Primitives.h"

namespace Lime
{
    class Xdata
    {
    public:
        DLL_EXPORT Xdata(const bool intersects, const float distance);

        DLL_EXPORT bool DoesIntersect();
        DLL_EXPORT float GetDistance();
    private:
        bool m_Intersects;
        float m_Distance;
    };
}

