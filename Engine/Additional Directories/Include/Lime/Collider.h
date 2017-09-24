#pragma once
#include "Primitives.h"
struct Collider
{
    //numbers in terms of grams/ meters
    float m_Area = 1;
    float m_Mass = 10;
    float m_DragCoefficient = 1.05f;
    float m_Velocity = 0.0f;
    float m_Acceleration = 0.0f;
    glm::vec3 m_Position = glm::vec3(0.0f,0.0f,0.0f);
};


