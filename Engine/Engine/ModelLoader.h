#pragma once
#include "Primitives.h"

namespace Lime
{
    class ModelLoader
    {
    public:
        DLL_EXPORT static std::shared_ptr<ModelData> LoadModel(std::string filename, unsigned int id);
        DLL_EXPORT static std::shared_ptr<ModelData> LoadModel2(std::string filename, unsigned int id);
        DLL_EXPORT static std::shared_ptr<ModelData> CreateTriangle(int size);
        DLL_EXPORT static std::shared_ptr<ModelData> CreateTriangle2();
        DLL_EXPORT static std::shared_ptr<ModelData> CreateCircle(float radius, float width, unsigned int tesselation, glm::vec3 color = glm::vec3(1,1,1));
        DLL_EXPORT static std::shared_ptr<ModelData> CreatePlane(unsigned int tesselation, const unsigned int length, const unsigned int width);
        DLL_EXPORT static std::shared_ptr<ModelData> CreateSimplePlane(const unsigned int width, const unsigned int length, glm::vec3 color = glm::vec3(0, 0, 0));
        DLL_EXPORT static std::shared_ptr<ModelData> CreateCube(float width,float height, glm::vec3 color = glm::vec3(1, 1, 1));
        DLL_EXPORT static std::shared_ptr<ModelData> LoadCubeMapCube(const float size);
    private:
        static unsigned int AssignID();
        static std::vector<Vertex> CreateCircleVertexArray(float radius, unsigned int tesselation, glm::vec3 color);
        static std::vector<Vertex> CreateCubeSide(float width, float height, glm::vec3 normal, glm::vec3 color);
    };


}