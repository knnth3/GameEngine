#pragma once
#include <vector>
#include <memory>
#include <glm\glm.hpp>
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <string>

typedef unsigned int UINT;
typedef const unsigned int CUint;
typedef unsigned char byte;
typedef std::vector<byte> BUFFER;
typedef uint16_t Texture;

//For later use
////API Types
//#define API_NONE 0x00
//#define API_DIRECTX 0x01
//#define API_OPENGL 0x02
//#define API_INUSE API_NONE


//OS Types
#define OS_UNDEFINED 0x00
#define OS_WINDOWS 0x01
#define OS_APPLE 0x02
#define OS_MINGW 0x03
#define OS_LINUX 0x04

#if defined(_WIN32)
#define PLATFORM OS_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM OS_APPLE
#elif defined(__MINGW32__)
#define PLATFORM OS_MINGW
#elif defined(__linux__)
#define OS_LINUX
#else
#define PLATFORM OS_UNDEFINED
#endif


#if PLATFORM == OS_WINDOWS
#endif


#define DLL_EXPORT __declspec(dllexport)
#define PI 3.1415927f
#undef DrawText


namespace Lime
{
	class DiffuseLight
	{
	public:
		DLL_EXPORT DiffuseLight() = default;
		DLL_EXPORT void SetDirection(const glm::vec3 dir);
		DLL_EXPORT void SetColor(const glm::vec4 color);
		DLL_EXPORT const glm::vec3 GetDirection() const;
		DLL_EXPORT const glm::vec4 GetColor() const;
	private:
		glm::vec3 m_direction;
		glm::vec4 m_color;
	};
    struct Vertex
    {
		DLL_EXPORT Vertex() = default;
		DLL_EXPORT Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz);
		DLL_EXPORT Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 normal);

        glm::vec3 m_position;
		glm::vec2 m_uv;
		glm::vec3 m_normal;
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