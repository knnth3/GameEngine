#include "pch.h"
#include "SpotLightDiffuse.h"
#include "Common\DirectXHelper.h"
#include <Graphics\BrushManager.h>


using namespace Microsoft::WRL;
using namespace Graphics;

Effects::SpotLightDiffuse::SpotLightDiffuse()
{
	m_lightPoint = D2D1::Vector3F(300.0f, 300.0f, 100.0f);
	m_focusPoint = D2D1::Vector3F(0.0f, 0.0f, 0.0f);
	m_color = D2D1::Vector3F(1.0f, 1.0f, 1.0f);
	m_focus = 1.0f;
	m_diffuseConstatnt = 1.0f;
}

void Effects::SpotLightDiffuse::SetLightPosition(glm::vec2 pos)
{
	m_lightPoint.x = pos.x;
	m_lightPoint.y = pos.y;
}

void Effects::SpotLightDiffuse::SetFocusPosition(glm::vec2 pos)
{
	m_focusPoint.x = pos.x;
	m_focusPoint.y = pos.y;
}

void Effects::SpotLightDiffuse::SetColor(glm::vec3 color)
{
	m_color.x = color.x;
	m_color.y = color.y;
	m_color.z = color.z;
}

void Effects::SpotLightDiffuse::SetFocus(float val)
{
	if(val >= 0.0f && val <= 200.0f)
		m_focus = val;
}

void Effects::SpotLightDiffuse::SetDiffuseConstant(float val)
{
	if(val >= 0.0f && val <= 10000.0f)
		m_diffuseConstatnt = val;
}

void Effects::SpotLightDiffuse::Create(ID2D1DeviceContext * context)
{

	//Diffuse
	if (!m_beginEffect)
	{
		DX::ThrowIfFailed(
			context->CreateEffect(CLSID_D2D1SpotDiffuse, &m_beginEffect)
		);
	}

	DX::ThrowIfFailed(
		m_beginEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_LIGHT_POSITION, m_lightPoint)
	);

	DX::ThrowIfFailed(
		m_beginEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_POINTS_AT, m_focusPoint)
	);

	DX::ThrowIfFailed(
		m_beginEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_FOCUS, m_focus)
	);

	DX::ThrowIfFailed(
		m_beginEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_DIFFUSE_CONSTANT, m_diffuseConstatnt)
	);

	DX::ThrowIfFailed(
		m_beginEffect->SetValue(D2D1_SPOTDIFFUSE_PROP_COLOR, m_color)
	);
	m_finalEffect = m_beginEffect;
	//////Specular
	//if (!m_finalEffect)
	//{
	//	DX::ThrowIfFailed(
	//		context->CreateEffect(CLSID_D2D1GaussianBlur, &m_finalEffect)
	//	);
	//}

	//m_finalEffect->SetInputEffect(0, m_beginEffect.Get());
}
