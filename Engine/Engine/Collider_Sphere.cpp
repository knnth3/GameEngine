#include "Collider_Sphere.h"

namespace Lime
{
    Collider_Sphere::Collider_Sphere()
    {
    }
    const glm::vec3 Collider_Sphere::GetCenter()
    {
        return m_Center;
    }
    const float Collider_Sphere::GetRadius()
    {
        return m_Radius;
    }
    Xdata Collider_Sphere::IntersectOther(Collider_Sphere & other)
    {

        float radiusDistance = m_Radius + other.GetRadius();
        float centerDistance = static_cast<float>((other.GetCenter() - m_Center).length());
        float distanceBetween = centerDistance - radiusDistance;

        return Xdata(centerDistance < radiusDistance, distanceBetween);
    }
}