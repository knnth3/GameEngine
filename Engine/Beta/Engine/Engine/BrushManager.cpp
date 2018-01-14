#include "BrushManager.h"

using namespace Microsoft::WRL;
using namespace std;

std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap>> Engine::BrushManager::m_images;
std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> Engine::BrushManager::m_solidBrushes;
std::vector<Microsoft::WRL::ComPtr<ID2D1BitmapBrush1>> Engine::BrushManager::m_imageBrushes;
std::shared_ptr<Engine::Effect_2D> Engine::BrushManager::m_effect;
ID2D1Factory3* Engine::BrushManager::m_2DFactory = nullptr;
ID2D1DeviceContext* Engine::BrushManager::m_2DDeviceContext = nullptr;
IWICImagingFactory2* Engine::BrushManager::m_wicFactory = nullptr;

bool Engine::BrushManager::Initialize(ID2D1Factory3* factory_2D, ID2D1DeviceContext* deviceContext_2D, IWICImagingFactory2* wicFactory)
{
	m_2DFactory = factory_2D;
	m_2DDeviceContext = deviceContext_2D;
	m_wicFactory = wicFactory;
	if (m_2DFactory && m_2DDeviceContext && m_wicFactory)
		return true;

	return false;
}

void Engine::BrushManager::Reset()
{
	SafeClose(m_images);
	SafeClose(m_solidBrushes);
	SafeClose(m_imageBrushes);
}

int Engine::BrushManager::CreateNewBrush(glm::vec4 color)
{
	size_t newID = FindNextEmpty(m_solidBrushes);
	D2D1_COLOR_F brush;
	brush.r = color.r;
	brush.g = color.g;
	brush.b = color.b;
	brush.a = color.a;
	ThrowIfFailed(
		m_2DDeviceContext->CreateSolidColorBrush(
			brush,
			&m_solidBrushes[newID]
		)
	);

	return (int)newID;
}

int Engine::BrushManager::CreateNewImageBrush(const int imageID, std::wstring& str)
{
	size_t newID = FindNextEmpty(m_imageBrushes);
	auto bmp = GetImage(imageID);
	if (!bmp)
	{
		str = L"Image with id: " + to_wstring(imageID) + L" does not exist. Image brush was not created.";
		return -1;
	}
	m_2DDeviceContext->CreateBitmapBrush(bmp.Get(), &m_imageBrushes[newID]);
	m_imageBrushes[newID]->SetExtendModeX(D2D1_EXTEND_MODE_WRAP);
	m_imageBrushes[newID]->SetExtendModeY(D2D1_EXTEND_MODE_WRAP);
	return (int)newID;
}

int Engine::BrushManager::CreateNewImage(std::string filename, std::wstring& str)
{
	std::wstring path = std::wstring(filename.begin(), filename.end());
	size_t newID = FindNextEmpty(m_images);
	LoadImageFromFile(path, m_images[newID], str);
	return (int)newID;
}

void Engine::BrushManager::AddEffect(std::shared_ptr<Effect_2D> effect)
{
	m_effect = effect;
}

void Engine::BrushManager::DeleteBrush(const int ID)
{
	SafeClose(m_solidBrushes, ID);
}

void Engine::BrushManager::DeleteImageBrush(const int ID)
{
	SafeClose(m_imageBrushes, ID);
}

void Engine::BrushManager::DeleteImage(const int ID)
{
	SafeClose(m_images, ID);
}

void Engine::BrushManager::ClearEffect()
{
	m_effect->Reset();
	m_effect = nullptr;
}

const bool Engine::BrushManager::GetImageDimensions(const int ID, glm::vec2& dims)
{
	if (m_images.size() > ID && ID >= 0)
	{
		D2D_SIZE_F size = m_images[ID]->GetSize();
		dims.x = size.width;
		dims.y = size.height;
		return true;
	}
	return false;
}

const std::shared_ptr<Engine::Effect_2D> Engine::BrushManager::GetEffect()
{
	return m_effect;
}

const Microsoft::WRL::ComPtr<ID2D1Bitmap> Engine::BrushManager::GetImage(int ID)
{
	if (m_images.size() > ID && ID >= 0)
		return m_images[ID];

	return nullptr;
}

const Microsoft::WRL::ComPtr<ID2D1BitmapBrush1> Engine::BrushManager::GetImageBrush(int ID)
{
	if (m_imageBrushes.size() > ID && ID >= 0)
		return m_imageBrushes[ID];

	return nullptr;
}

const Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> Engine::BrushManager::GetBrush(int ID)
{
	if (m_solidBrushes.size() > ID && ID >= 0)
		return m_solidBrushes[ID];

	return nullptr;
}

bool Engine::BrushManager::LoadImageFromFile(std::wstring filename, Microsoft::WRL::ComPtr<ID2D1Bitmap>& bitmap, std::wstring& str)
{
	HRESULT result;
	ComPtr<IWICBitmapDecoder> decoder;
	ComPtr<IWICFormatConverter> converter;
	ComPtr<IWICBitmapFrameDecode> frame;
	result = m_wicFactory->CreateDecoderFromFilename(
		filename.c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&decoder
	);

	if (!SUCCEEDED(result))
	{
		str = L"Could not load file: " + filename;
		return false;
	}

	result = decoder->GetFrame(0, &frame);

	if (!SUCCEEDED(result))
	{
		str = L"Could not load file: " + filename;
		return false;
	}

	result = m_wicFactory->CreateFormatConverter(&converter);

	if (!SUCCEEDED(result))
	{
		str = L"Could not load file: " + filename;
		return false;
	}

	result = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);

	if (!SUCCEEDED(result))
	{
		str = L"Could not load file: " + filename;
		return false;
	}

	result = m_2DDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), NULL, &bitmap);

	if (!SUCCEEDED(result))
	{
		str = L"Could not load file: " + filename;
		return false;
	}

	frame.Reset();
	converter.Reset();
	decoder.Reset();
	return true;
}

template<class T>
int Engine::BrushManager::FindNextEmpty(std::vector<T>& arr)
{
	for (size_t it = 0; it < arr.size(); it++)
		if (arr[it] == nullptr)
			return (int)it;

	arr.resize(arr.size() + 1);
	return (int)arr.size() - 1;
}

template<class T>
void Engine::BrushManager::SafeClose(std::vector<Microsoft::WRL::ComPtr<T>>& arr)
{
	for (auto& item : arr)
		item.Reset();
	arr.clear();
}

template<class T>
void Engine::BrushManager::SafeClose(std::vector<Microsoft::WRL::ComPtr<T>>& arr, int ID)
{
	if (arr.size() > ID)
	{
		arr[ID].Reset();
		arr[ID] = nullptr;
	}
}
