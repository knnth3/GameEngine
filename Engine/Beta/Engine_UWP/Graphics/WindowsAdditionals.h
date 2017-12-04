#pragma once
#include "DllSettings.h"
#include <Dwrite_3.h>
#include <d2d1_3.h>
#include <wrl.h>

#define CLOSE_COM_PTR(ptr) \
	if(ptr) { ptr->Release(); ptr = nullptr;}

GRAPHICS_DLL_API inline bool Check(bool x, LPCWSTR lpctstr, LPCWSTR message);
GRAPHICS_DLL_API inline bool CheckSuccess(HRESULT hresult, LPCWSTR message);
GRAPHICS_DLL_API inline std::wstring GetLastErrorAsString();

namespace Graphics
{
	// Helper utility converts D3D API failures into exceptions.
	GRAPHICS_DLL_API inline void ThrowIfFailed(HRESULT hr);
}