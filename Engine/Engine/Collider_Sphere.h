#pragma once
#include "Primitives.h"
#include "Xdata.h"
namespace Lime
{
    class Collider_Sphere
    {
    public:
        DLL_EXPORT Collider_Sphere();

        DLL_EXPORT const glm::vec3 GetCenter();
        DLL_EXPORT const float GetRadius();

        DLL_EXPORT Xdata IntersectOther(Collider_Sphere& other);
    private:
        glm::vec3 m_Center;
        float m_Radius;
    };
}

