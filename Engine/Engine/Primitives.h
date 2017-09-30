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
typedef uint16_t Texture;

#define DLL_EXPORT __declspec(dllexport)
#undef DrawText


namespace Lime
{
	struct MatrixBuffer
	{
		glm::mat4 world;
		glm::mat4 view;
		glm::mat4 projection;
	};

    struct Vertex
    {
		DLL_EXPORT Vertex() = default;
		DLL_EXPORT Vertex(float x, float y, float z, float u, float v);

        glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
    };

	struct ModelData
	{
	public:
		DLL_EXPORT ModelData();
		DLL_EXPORT size_t VertexBufferSize();
		DLL_EXPORT size_t IndexBufferSize();

		std::string renderType = "Mesh";
		unsigned int m_ObjectID = 0;
		unsigned int m_VertOffset = 0;
		unsigned int m_IndiciOffset = 0;
		std::vector<Vertex> m_Verticies;
		std::vector<uint32_t> m_Indicies;
	private:
		static const unsigned int GetNewID();

	};

	struct Model3D
	{
		DLL_EXPORT Model3D();
		DLL_EXPORT void Scale(const float x, const float y, const float z);
		DLL_EXPORT void Translate(const float x, const float y, const float z);
		DLL_EXPORT void Translate(glm::vec3 pos);
		//In Radians
		DLL_EXPORT void Rotate(float x, float y, float z);
		DLL_EXPORT void RotateAtOrigin(float x, float y, float z);
		DLL_EXPORT void Color(float r, float g, float b);
		DLL_EXPORT void Color(float r, float g, float b, float a);
		DLL_EXPORT void SetOpacity(float alpha);
		DLL_EXPORT void SetOffset(float off);
		DLL_EXPORT void SetTexture(Texture tex);
		DLL_EXPORT glm::vec3 GetPosition()const;
		DLL_EXPORT glm::mat4 GetLocalToWorld();
		DLL_EXPORT glm::vec4 GetColor();
		DLL_EXPORT Texture GetTexture();
		std::shared_ptr<ModelData> m_Data;
		void * m_ptr;
	private:
		void CreateLocalToWorld();
		glm::mat4 m_outRotation;
		glm::mat4 m_inRotation;
		glm::mat4 m_scaleMatrix;
		glm::mat4 m_translation;
		glm::mat4 m_localToWorld;
		glm::vec3 m_Position;
		glm::vec3 m_offset;
		glm::vec3 m_scale;
		glm::vec4 m_color;
		uint16_t m_texture;
	};

}