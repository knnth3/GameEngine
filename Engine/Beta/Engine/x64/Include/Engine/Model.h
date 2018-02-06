#pragma once
#include "Animator.h"


typedef std::shared_ptr<glm::vec3> vec3_ptr;

namespace Engine
{

	enum DrawStyle
	{
		DRAW_STYLE_NORMAL = 0,
		DRAW_STYLE_CUBEMAP = 1,
		DRAW_STYLE_CULL_FRONT = 2,
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
		__engine_decl Model(int meshID);
		__engine_decl ~Model();
		__engine_decl virtual void UseViewMatrix(bool val) final;
		__engine_decl virtual bool IsUsingViewMatrix() final;

		//Set Functions

		__engine_decl virtual void SetTexture(const std::string& textureName) final;
		__engine_decl virtual void SetMesh(int ID) final;
		__engine_decl virtual void SetSecondaryMesh(int ID) final;
		__engine_decl virtual void SetDrawStyle(int style) final;
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
		__engine_decl virtual void Update(double elapsed);
		__engine_decl virtual void ToggleAnimation();

		//Get Functions
		__engine_decl virtual const AnimTransformPtr GetTransforms()const final;
		__engine_decl virtual int GetObjectID()const final;
		__engine_decl virtual int GetDrawStyle()const final;
		__engine_decl virtual std::string GetTexture()const final;
		__engine_decl virtual int GetMesh()const final;
		__engine_decl virtual int GetSecondaryMesh()const final;
		__engine_decl virtual const vec3_ptr GetPosition()const;
		__engine_decl virtual glm::vec3 GetScale()const;
		__engine_decl virtual glm::mat4 GetModelMatrix()const;
		__engine_decl virtual glm::vec4 GetColor()const;
		__engine_decl virtual glm::vec4 GetTextureBounds()const;

	private:
		void Rotate(float& original, const float& amount);
		glm::mat4 GetRotationMatrix(glm::vec3 rotation)const;
		void InitializeAnimator();

		bool m_bUseViewMatrix;
		int m_objectID;
		int m_meshID;
		int m_meshID2;
		int m_drawStyle;
		std::string m_texture;
		glm::mat4 m_scaleMatrix;
		vec3_ptr m_position;
		glm::vec3 m_object_rotation;
		glm::vec3 m_world_rotation;
		glm::vec3 m_scale;
		glm::vec4 m_color;
		glm::vec4 m_textureBounds;
		std::unique_ptr<Animator> m_animator;
	};
}