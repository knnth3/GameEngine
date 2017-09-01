#include "Xdata.h"

namespace Lime
{
    Xdata::Xdata(const bool intersects, const float distance) :
        m_Intersects(intersects),
        m_Distance(distance)
    {
    }
    bool Xdata::DoesIntersect()
    {
        return m_Intersects;
    }
    float Xdata::GetDistance()
    {
        return m_Distance;
    }
}
