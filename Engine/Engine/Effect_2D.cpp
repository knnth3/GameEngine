#include "Effect_2D.h"

Engine::Effect_2D::Effect_2D()
{
	m_beginEffect = nullptr;
	m_finalEffect = nullptr;
}

void Engine::Effect_2D::Create(ID2D1DeviceContext * context)
{

}

Microsoft::WRL::ComPtr<ID2D1Effect> Engine::Effect_2D::GetBeginEffect()
{
	return m_beginEffect;
}

Microsoft::WRL::ComPtr<ID2D1Effect> Engine::Effect_2D::GetFinalEffect()
{
	return m_finalEffect;
}

void Engine::Effect_2D::Reset()
{
	m_beginEffect.Reset();
	m_finalEffect.Reset();
}