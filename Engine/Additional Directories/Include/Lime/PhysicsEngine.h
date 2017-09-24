#pragma once
#include "Primitives.h"
#include "Collider.h"
class PhysicsEngine
{
public:
    DLL_EXPORT PhysicsEngine();
    DLL_EXPORT PhysicsEngine(const float gravity, const glm::vec3 DirectionNormal);
    DLL_EXPORT int GetOpenSlot();
    DLL_EXPORT int AddObject(const std::shared_ptr<Collider>& col);
    DLL_EXPORT void DeleteObject(int ID);
    DLL_EXPORT void Update(float dTime);

    DLL_EXPORT void SetGravity(float gravity);
    DLL_EXPORT void SetGravityDir(glm::vec3 dir);
 
    DLL_EXPORT float GetGravity();
    DLL_EXPORT glm::vec3 GetGravityDir();
private:
    float m_airDesnity = 0.001225f;
    float m_Gravity;
    glm::vec3 m_GNormal;
    std::vector<std::shared_ptr<Collider>> m_Container;


};

