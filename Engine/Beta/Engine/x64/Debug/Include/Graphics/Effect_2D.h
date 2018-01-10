#pragma once
#include "WindowsAdditionals.h"


namespace Graphics
{
	class Effect_2D
	{
	public:
		GRAPHICS_DLL_API Effect_2D();
		GRAPHICS_DLL_API virtual void Create(ID2D1DeviceContext* context);
		GRAPHICS_DLL_API virtual Microsoft::WRL::ComPtr<ID2D1Effect> GetBeginEffect()final;
		GRAPHICS_DLL_API virtual Microsoft::WRL::ComPtr<ID2D1Effect> GetFinalEffect()final;
		GRAPHICS_DLL_API virtual void Reset()final;

	protected:

		Microsoft::WRL::ComPtr<ID2D1Effect> m_beginEffect;
		Microsoft::WRL::ComPtr<ID2D1Effect> m_finalEffect;

	};

}