#pragma once
#include "Mesh.h"


typedef std::shared_ptr<glm::vec3> vec3_ptr;

namespace Graphics
{

	enum DrawStyle
	{
		TRIANGLE_3D = 0,
		TRIANGLE_2D
	};
	enum RSS_STYLES
	{
		RSS_DEFAULT = 0,
		RSS_NO_CULL,
		RSS_WIREFRAME
	};

	class IDManager
	{
	public:
		static void AssignID(int& id);
		static void RemoveID(int id);

	private:
		static std::vector<int> currentIDs;
	};

	class Model
	{
	public:
		//Constructors, destructors, and final functions

		GRAPHICS_DLL_API Model();
		GRAPHICS_DLL_API Model(MeshID mesh);
		GRAPHICS_DLL_API ~Model();
		GRAPHICS_DLL_API virtual void UseViewMatrix(bool val) final;
		GRAPHICS_DLL_API virtual bool IsUsingViewMatrix() final;

		//Set Functions

		GRAPHICS_DLL_API virtual void SetTexture(const std::string& textureName) final;
		GRAPHICS_DLL_API virtual void SetMesh(MeshID meshName) final;
		GRAPHICS_DLL_API virtual void SetDrawStyle(DrawStyle style) final;
		GRAPHICS_DLL_API virtual void SetRSS(RSS_STYLES style) final;
		GRAPHICS_DLL_API virtual void Scale(const float x, const float y, const float z);
		GRAPHICS_DLL_API virtual void Scale(glm::vec3 scale);
		GRAPHICS_DLL_API virtual void SetPosition(const float x, const float y, const float z);
		GRAPHICS_DLL_API virtual void SetPosition(glm::vec3 pos);
		GRAPHICS_DLL_API virtual void SetRelativeRotation(float x, float y, float z);
		GRAPHICS_DLL_API virtual void SetRelativeRotation(glm::vec3 rotation);
		GRAPHICS_DLL_API virtual void SetWorldRotation(float x, float y, float z);
		GRAPHICS_DLL_API virtual void SetWorldRotation(glm::vec3 rotation);
		GRAPHICS_DLL_API virtual void RotateRelative(float x, float y, float z);
		GRAPHICS_DLL_API virtual void RotateRelative(glm::vec3 rotation);
		GRAPHICS_DLL_API virtual void RotateWorld(float x, float y, float z);
		GRAPHICS_DLL_API virtual void RotateWorld(glm::vec3 rotation);
		GRAPHICS_DLL_API virtual void SetColor(float r, float g, float b);
		GRAPHICS_DLL_API virtual void SetColor(float r, float g, float b, float a);
		GRAPHICS_DLL_API virtual void SetColor(glm::vec4 color);
		GRAPHICS_DLL_API virtual void SetOpacity(float alpha);
		GRAPHICS_DLL_API virtual void SetTextureBounds(float length, float width, float xoffset, float yoffset);

		//Get Functions

		GRAPHICS_DLL_API virtual int GetUniqueID()const final;
		GRAPHICS_DLL_API virtual DrawStyle GetDrawStyle()const final;
		GRAPHICS_DLL_API virtual RSS_STYLES GetRSS()const final;
		GRAPHICS_DLL_API virtual std::string GetTexture()const final;
		GRAPHICS_DLL_API virtual MeshID GetMesh()const final;
		GRAPHICS_DLL_API virtual const vec3_ptr GetPosition()const;
		GRAPHICS_DLL_API virtual glm::vec3 GetScale()const;
		GRAPHICS_DLL_API virtual glm::mat4 GetModelMatrix()const;
		GRAPHICS_DLL_API virtual glm::vec4 GetColor()const;
		GRAPHICS_DLL_API virtual glm::vec4 GetTextureBounds()const;

	private:
		glm::mat4 GetRotationMatrix(glm::vec3 rotation)const;

		bool m_bUseViewMatrix;
		int m_uniqueID;
		MeshID m_mesh;
		std::string m_texture;
		DrawStyle m_drawStyle;
		RSS_STYLES m_rssStyle;
		glm::mat4 m_scaleMatrix;
		vec3_ptr m_position;
		glm::vec3 m_object_rotation;
		glm::vec3 m_world_rotation;
		glm::vec3 m_scale;
		glm::vec4 m_color;
		glm::vec4 m_textureBounds;
	};
}