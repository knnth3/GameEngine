#include "WindowsAdditionals.h"

bool Check(bool x, LPCWSTR lpctstr, LPCWSTR message)
{
	if (!(x))
	{
		::MessageBox(0, message, lpctstr, MB_OK);
		return false;
	}
	else
		return true;
}

bool CheckSuccess(HRESULT hresult, LPCWSTR message)
{
	_com_error err(hresult);
	return Check(SUCCEEDED(hresult), err.ErrorMessage(), message);
}

std::wstring GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::wstring(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	int size = ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::wstring message;
	message.resize(30);
	int bufferSize = (int)message.size();
	::MultiByteToWideChar(CP_ACP, 0, messageBuffer, size, &message[0], bufferSize);

	//Free the buffer.
	::LocalFree(messageBuffer);

	return message;
}

void Graphics::ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception();
	}
}