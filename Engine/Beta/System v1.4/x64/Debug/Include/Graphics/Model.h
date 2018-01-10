#pragma once
#include "Mesh.h"

namespace Graphics
{
	//Typedefs
	typedef int32_t TextureID;

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
		GRAPHICS_DLL_API Model(const MeshID id);
		GRAPHICS_DLL_API ~Model();
		GRAPHICS_DLL_API virtual void SetTexture(TextureID id) final;
		GRAPHICS_DLL_API virtual void SetMesh(MeshID id) final;
		GRAPHICS_DLL_API virtual void SetDrawStyle(DrawStyle style) final;
		GRAPHICS_DLL_API virtual void SetRSS(RSS_STYLES style) final;
		GRAPHICS_DLL_API virtual TextureID GetTexture() final;
		GRAPHICS_DLL_API virtual MeshID GetMesh() final;
		GRAPHICS_DLL_API virtual int GetUniqueID() final;
		GRAPHICS_DLL_API virtual DrawStyle GetDrawStyle() final;
		GRAPHICS_DLL_API virtual RSS_STYLES GetRSS() final;
		GRAPHICS_DLL_API virtual void UseViewMatrix(bool val) final;
		GRAPHICS_DLL_API virtual bool IsUsingViewMatrix() final;

		//Set Functions

		GRAPHICS_DLL_API virtual void Scale(const float x, const float y, const float z);
		GRAPHICS_DLL_API virtual void Scale(glm::vec3 scale);
		GRAPHICS_DLL_API virtual void SetPosition(const float x, const float y, const float z);
		GRAPHICS_DLL_API virtual void SetPosition(glm::vec3 pos);
		GRAPHICS_DLL_API virtual void Rotate(float x, float y, float z);
		GRAPHICS_DLL_API virtual void Rotate(glm::vec3 rotation);
		GRAPHICS_DLL_API virtual void RotateAtOrigin(float x, float y, float z);
		GRAPHICS_DLL_API virtual void SetColor(float r, float g, float b);
		GRAPHICS_DLL_API virtual void SetColor(float r, float g, float b, float a);
		GRAPHICS_DLL_API virtual void SetColor(glm::vec4 color);
		GRAPHICS_DLL_API virtual void SetOpacity(float alpha);
		GRAPHICS_DLL_API virtual void SetTextureBounds(float length, float width, float xoffset, float yoffset);

		//Get Functions

		GRAPHICS_DLL_API virtual glm::vec3 GetPosition();
		GRAPHICS_DLL_API virtual glm::vec3 GetScale();
		GRAPHICS_DLL_API virtual glm::mat4 GetModelMatrix();
		GRAPHICS_DLL_API virtual glm::vec4 GetColor();
		GRAPHICS_DLL_API virtual glm::vec4 GetTextureBounds();

	private:
		void Initialize();
		void CreateLocalToWorld();
		void RotateMatrix(glm::mat4& matrix, glm::vec3 rotations);

		bool m_bUseViewMatrix;
		int m_uniqueID;
		MeshID m_meshID;
		TextureID m_textureID;
		DrawStyle m_drawStyle;
		RSS_STYLES m_rssStyle;
		glm::mat4 m_outRotation;
		glm::mat4 m_inRotation;
		glm::mat4 m_scaleMatrix;
		glm::mat4 m_translation;
		glm::mat4 m_localToWorld;
		glm::vec3 m_position;
		glm::vec3 m_scale;
		glm::vec4 m_color;
		glm::vec4 m_textureBounds;
	};
}