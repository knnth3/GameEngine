#pragma once
#include "Mesh.h"


typedef std::shared_ptr<glm::vec3> vec3_ptr;

namespace Engine
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

		__engine_decl Model();
		__engine_decl Model(MeshID mesh);
		__engine_decl ~Model();
		__engine_decl virtual void UseViewMatrix(bool val) final;
		__engine_decl virtual bool IsUsingViewMatrix() final;

		//Set Functions

		__engine_decl virtual void SetTexture(const std::string& textureName) final;
		__engine_decl virtual void SetMesh(MeshID meshName) final;
		__engine_decl virtual void SetDrawStyle(DrawStyle style) final;
		__engine_decl virtual void SetRSS(RSS_STYLES style) final;
		__engine_decl virtual void Scale(const float x, const float y, const float z);
		__engine_decl virtual void Scale(glm::vec3 scale);
		__engine_decl virtual void SetPosition(const float x, const float y, const float z);
		__engine_decl virtual void SetPosition(glm::vec3 pos);
		__engine_decl virtual void SetRelativeRotation(float x, float y, float z);
		__engine_decl virtual void SetRelativeRotation(glm::vec3 rotation);
		__engine_decl virtual void SetWorldRotation(float x, float y, float z);
		__engine_decl virtual void SetWorldRotation(glm::vec3 rotation);
		__engine_decl virtual void RotateRelative(float x, float y, float z);
		__engine_decl virtual void RotateRelative(glm::vec3 rotation);
		__engine_decl virtual void RotateWorld(float x, float y, float z);
		__engine_decl virtual void RotateWorld(glm::vec3 rotation);
		__engine_decl virtual void SetColor(float r, float g, float b);
		__engine_decl virtual void SetColor(float r, float g, float b, float a);
		__engine_decl virtual void SetColor(glm::vec4 color);
		__engine_decl virtual void SetOpacity(float alpha);
		__engine_decl virtual void SetTextureBounds(float length, float width, float xoffset, float yoffset);

		//Get Functions

		__engine_decl virtual int GetUniqueID()const final;
		__engine_decl virtual DrawStyle GetDrawStyle()const final;
		__engine_decl virtual RSS_STYLES GetRSS()const final;
		__engine_decl virtual std::string GetTexture()const final;
		__engine_decl virtual MeshID GetMesh()const final;
		__engine_decl virtual const vec3_ptr GetPosition()const;
		__engine_decl virtual glm::vec3 GetScale()const;
		__engine_decl virtual glm::mat4 GetModelMatrix()const;
		__engine_decl virtual glm::vec4 GetColor()const;
		__engine_decl virtual glm::vec4 GetTextureBounds()const;

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