#pragma once
#include "DllSettings.h"
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <atlbase.h>

#define CLOSE_COM_PTR(ptr) \
	if(ptr) { ptr->Release(); ptr = nullptr;}

__engine_decl inline bool Check(bool x, LPCWSTR lpctstr, LPCWSTR message);
__engine_decl inline bool CheckSuccess(HRESULT hresult, LPCWSTR message);
__engine_decl inline void OpenDialog(LPCWSTR title, LPCWSTR message);
__engine_decl inline std::wstring GetLastErrorAsString();


namespace Engine
{
	// Helper utility converts D3D API failures into exceptions.
	__engine_decl inline void ThrowIfFailed(HRESULT hr);
}