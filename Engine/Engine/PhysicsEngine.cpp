#include "PhysicsEngine.h"



PhysicsEngine::PhysicsEngine()
{
    m_Gravity = 9.8f;
    m_GNormal = glm::vec3(0.0f, -1.0f, 0.0f);
}

PhysicsEngine::PhysicsEngine(const float gravity, const glm::vec3 DirectionNormal)
{
    m_Gravity = gravity;
    m_GNormal = DirectionNormal;
}

int PhysicsEngine::GetOpenSlot()
{
    for (size_t pos = 0; pos < m_Container.size(); pos++)
    {
        if (m_Container[pos] == nullptr)
        {
            return static_cast<int>(pos);
        }
    }
    return -1;
}

int PhysicsEngine::AddObject(const std::shared_ptr<Collider>& col)
{
    int ID = GetOpenSlot();
    if (ID == -1)
    {
        m_Container.push_back(col);
    }
    else
    {
        m_Container[ID] = col;
    }
    return ID;
}

void PhysicsEngine::DeleteObject(int ID)
{
    if (ID >= 0 && ID < m_Container.size())
    {
        m_Container[ID] = nullptr;
    }
}

void PhysicsEngine::Update(float dTime)
{
    for each(auto object in m_Container)
    {
        float acceleration = 0;
        float terminalVelocity = sqrtf((2.0f * object->m_Mass * m_Gravity)/(m_airDesnity * object->m_Area * object->m_DragCoefficient));
        if (object->m_Velocity <= terminalVelocity)
        {
            acceleration = m_Gravity;
        }
        float distance = (object->m_Velocity * dTime) + ((acceleration * powf(dTime, 2) / 2));
        float finalVelocity = object->m_Velocity + (acceleration*dTime);
        object->m_Velocity = finalVelocity;

        object->m_Position = object->m_Position + (m_GNormal*distance);

    }
}

void PhysicsEngine::SetGravity(float gravity)
{
    m_Gravity = gravity;
}

void PhysicsEngine::SetGravityDir(glm::vec3 dir)
{
    m_GNormal = glm::normalize(dir);
}

float PhysicsEngine::GetGravity()
{
    return m_Gravity;
}

glm::vec3 PhysicsEngine::GetGravityDir()
{
    return m_GNormal;
}
