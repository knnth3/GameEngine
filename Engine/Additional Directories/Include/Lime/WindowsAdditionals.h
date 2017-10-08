#pragma once

#define Check(x, lpctstr) \
	if(!(x)) { MessageBox(0, lpctstr, L"Error", MB_OK);}

#define CheckSuccess(hresult) \
	{_com_error err(hresult); Check(SUCCEEDED(hresult), err.ErrorMessage());}

#define CLOSE_COM_PTR(ptr) \
	if(ptr) { ptr->Release(); ptr = nullptr;}