#pragma once
#include <wrl.h>
#include <Graphics\Effect_2D.h>

namespace Effects
{
	class SpotLightDiffuse:
		public Graphics::Effect_2D
	{
	public:
		SpotLightDiffuse();
		void SetLightPosition(glm::vec2 pos);
		void SetFocusPosition(glm::vec2 pos);
		void SetColor(glm::vec3 color);
		void SetFocus(float val);
		void SetDiffuseConstant(float val);
		virtual void Create(ID2D1DeviceContext* context)override;

	private:

		float m_focus;
		float m_diffuseConstatnt;
		D2D1_VECTOR_3F m_lightPoint;
		D2D1_VECTOR_3F m_focusPoint;
		D2D1_VECTOR_3F m_color;
	};


}