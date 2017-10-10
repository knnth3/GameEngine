#pragma once
#include "Shared.h"
#include "Collider.h"
class PhysicsEngine
{
public:
    AppDLL_API PhysicsEngine();
    AppDLL_API PhysicsEngine(const float gravity, const glm::vec3 DirectionNormal);
    AppDLL_API int GetOpenSlot();
    AppDLL_API int AddObject(const std::shared_ptr<Collider>& col);
    AppDLL_API void DeleteObject(int ID);
    AppDLL_API void Update(float dTime);

    AppDLL_API void SetGravity(float gravity);
    AppDLL_API void SetGravityDir(glm::vec3 dir);
 
    AppDLL_API float GetGravity();
    AppDLL_API glm::vec3 GetGravityDir();
private:
    float m_airDesnity = 0.001225f;
    float m_Gravity;
    glm::vec3 m_GNormal;
    std::vector<std::shared_ptr<Collider>> m_Container;


};

