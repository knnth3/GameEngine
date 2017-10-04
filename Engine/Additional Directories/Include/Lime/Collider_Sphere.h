#pragma once
#include "Lime.h"
#include "Xdata.h"
namespace Lime
{
    class Collider_Sphere
    {
    public:
        AppDLL_API Collider_Sphere();

        AppDLL_API const glm::vec3 GetCenter();
        AppDLL_API const float GetRadius();

        AppDLL_API Xdata IntersectOther(Collider_Sphere& other);
    private:
        glm::vec3 m_Center;
        float m_Radius;
    };
}

