#pragma once
#include "Model.h"


namespace Lime
{
	namespace Model
	{		
		class Model2D
		{
			friend class VertexLibrary;
			friend class Lime::DX11Graphics;
			friend class Lime::TextInfo;
		public:
			AppDLL_API Model2D();
			AppDLL_API Model2D(const glm::vec2 pos, const float length, const float width, const glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f});
			//Make sure to add to Renderer to work
			AppDLL_API void Draw();
			AppDLL_API void Scale(const float x, const float y, const float z);
			AppDLL_API void Scale(glm::vec3 scale);
			AppDLL_API void SetPosition(const float x, const float y);
			AppDLL_API void SetPosition(glm::vec2 pos);
			AppDLL_API void Rotate(float rad);
			AppDLL_API void SetColor(float r, float g, float b);
			AppDLL_API void SetColor(float r, float g, float b, float a);
			AppDLL_API void SetColor(glm::vec4 color);
			AppDLL_API void SetOpacity(float alpha);
			AppDLL_API void SetTexture(TextureID tex);
			AppDLL_API void SetLength(const float length);
			AppDLL_API void SetWidth(const float width);
			AppDLL_API glm::vec2 GetPosition()const;
			AppDLL_API glm::mat4 GetModelMatrix();
			AppDLL_API glm::vec4 GetColor();
			AppDLL_API TextureID GetTexture();
			AppDLL_API float GetLength();
			AppDLL_API float GetWidth();

		protected:
			MeshType m_meshType = MeshType::POINT;
			std::shared_ptr<MeshData2D> m_mesh;
			bool m_bDraw;
		private:
			void CreateLocalToWorld();
			void RotateMatrix(glm::mat4& matrix, glm::vec3 rotations);

			glm::mat4 m_outRotation;
			glm::mat4 m_scaleMatrix;
			glm::mat4 m_translation;
			glm::mat4 m_localToWorld;
			glm::vec3 m_scale;
			glm::vec4 m_color;
			glm::vec2 m_position;
			TextureID m_texture;
			float m_length;
			float m_width;
		};
	}
}
