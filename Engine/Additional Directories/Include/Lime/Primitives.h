#pragma once
#include <vector>
#include <memory>
#include <glm\glm.hpp>
#include <gl\glew.h>
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <string>

typedef unsigned int UINT;
typedef const unsigned int CUint;
typedef unsigned char byte;
typedef std::vector<byte> BUFFER;

#define MAX_TRANSFER_RATE 1024*16
#define DLL_EXPORT __declspec(dllexport)


namespace Lime
{

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;
    };

	struct Vertex2
	{
		DLL_EXPORT Vertex2(Vertex v);
		DLL_EXPORT Vertex2() = default;
		DLL_EXPORT Vertex2(float x, float y, float z, float u, float v);

		glm::vec3 position;
		glm::vec2 uv;
	};

    struct ModelData
    {
		DLL_EXPORT ModelData();
		DLL_EXPORT size_t VertexBufferSize();
		DLL_EXPORT size_t IndexBufferSize();

        unsigned int m_ObjectID = 0;
        std::vector<Vertex> m_Verticies;
        std::vector<GLshort> m_Indicies;


    };

	struct ModelData2
	{
	public:
		DLL_EXPORT ModelData2();
		DLL_EXPORT size_t VertexBufferSize();
		DLL_EXPORT size_t IndexBufferSize();

		std::string renderType = "Triangle";
		unsigned int m_ObjectID = 0;
		unsigned int m_VertOffset = 0;
		unsigned int m_IndiciOffset = 0;
		std::vector<Vertex2> m_Verticies;
		std::vector<uint32_t> m_Indicies;
	private:
		static const unsigned int GetNewID();

	};

	struct Model2
	{
		DLL_EXPORT Model2();
		DLL_EXPORT void Scale(const float x, const float y, const float z);
		DLL_EXPORT void Translate(const float x, const float y, const float z);
		//In Radians
		DLL_EXPORT void Rotate(float x, float y, float z);
		DLL_EXPORT void RotateAtOrigin(float x, float y, float z);
		DLL_EXPORT void Color(float r, float g, float b);
		DLL_EXPORT void Color(float r, float g, float b, float a);
		DLL_EXPORT void SetOpacity(float alpha);
		DLL_EXPORT glm::vec3 GetPosition()const;
		DLL_EXPORT glm::mat4 GetLocalToWorld();
		DLL_EXPORT glm::vec4 GetColor();
		std::shared_ptr<ModelData2> m_Data;
	private:
		void CreateLocalToWorld();
		glm::mat4 m_outRotation;
		glm::mat4 m_inRotation;
		glm::mat4 m_scale;
		glm::mat4 m_translation;
		glm::mat4 m_localToWorld;
		glm::vec3 m_Position;
		glm::vec4 m_color;
	};


    struct Model
    {
        Model() :
            m_Data(0),
            m_Position(0, 0, 0),
            m_Rotation(0, 0, 0)
        {
            m_Matrices = std::make_shared<std::vector<glm::mat4>>();
            m_Matrices->resize(2);
        }
        std::shared_ptr<std::vector<glm::mat4>> GetMatrices()
        {
            m_Matrices->at(0) = glm::translate(glm::mat4(), glm::vec3(m_Position.x, m_Position.y, m_Position.z));
            glm::mat4 rotX = glm::rotate(glm::mat4(), m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 rotY = glm::rotate(rotX, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 rotFinal = glm::rotate(rotY, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            m_Matrices->at(1) = rotFinal;
            return m_Matrices;
        }

        glm::vec3 GetPosition()const
        {
            return m_Position;
        }

        glm::vec3 GetRotation()const
        {
            return m_Rotation;
        }

        void Translate(const float x, const float y, const float z)
        {
            m_Position.x = x;
            m_Position.y = y;
            m_Position.z = z;
        }
        void SetRotation(float x, float y, float z)
        {
            float circle = 3.1415f * 2.0f;

            m_Rotation.x = x;
            m_Rotation.y = y;
            m_Rotation.z = z;

            if (m_Rotation.x >= circle)
                m_Rotation.x = circle;

            if (m_Rotation.y >= circle)
                m_Rotation.y = circle;

            if (m_Rotation.z >= circle)
                m_Rotation.z = circle;
        }
        void RotateX(const float angleInRad)
        {
            float circle = 3.1415f * 2.0f;
            m_Rotation.x += angleInRad;

            if (m_Rotation.x >= circle)
                m_Rotation.x = circle;
        }
        void RotateY(const float angleInRad)
        {
            float circle = 3.1415f * 2.0f;
            m_Rotation.y += angleInRad;

            if (m_Rotation.y >= circle)
                m_Rotation.y = circle;
        }
        void RotateZ(const float angleInRad)
        {
            float circle = 3.1415f * 2.0f;
            m_Rotation.z += angleInRad;

            if (m_Rotation.z >= circle)
                m_Rotation.z = circle;
        }
        void SetColor(float r, float g, float b)
        {
            for (int i = 0; i < m_Data->m_Verticies.size(); i++)
            {
                m_Data->m_Verticies.at(i).color = glm::vec3(r, g, b);
            }
        }
        std::shared_ptr<ModelData> m_Data;
    private:
        //First matrix is Translation
        //Second matrix is Rotation
        std::shared_ptr<std::vector<glm::mat4>> m_Matrices;
        glm::vec3 m_Rotation;
        glm::vec3 m_Position;
    };

}