#pragma once
#include "WindowsAdditionals.h"


namespace Graphics
{
	class Effect_2D
	{
	public:
		__engine_decl Effect_2D();
		__engine_decl virtual void Create(ID2D1DeviceContext* context);
		__engine_decl virtual Microsoft::WRL::ComPtr<ID2D1Effect> GetBeginEffect()final;
		__engine_decl virtual Microsoft::WRL::ComPtr<ID2D1Effect> GetFinalEffect()final;
		__engine_decl virtual void Reset()final;

	protected:

		Microsoft::WRL::ComPtr<ID2D1Effect> m_beginEffect;
		Microsoft::WRL::ComPtr<ID2D1Effect> m_finalEffect;

	};

}