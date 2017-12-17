#include "RenderBatch_2D.h"
#include "BrushManager.h"

using namespace Microsoft::WRL;


Graphics::RenderBatch_2D::RenderBatch_2D(IDWriteFactory3 * writeFactory, 
	ID2D1Factory3 * factory_2D, ID2D1DeviceContext * deviceContext_2D, IWICImagingFactory2* wicFactory)
{
	m_writeFactory = writeFactory;
	m_2DFactory = factory_2D;
	m_2DDeviceContext = deviceContext_2D;
	m_wicFactory = wicFactory;
}

Graphics::RenderBatch_2D::~RenderBatch_2D()
{

}

void Graphics::RenderBatch_2D::Initialize()
{
	ThrowIfFailed(
		m_2DFactory->CreateDrawingStateBlock(&m_stateBlock)
	);

	CreateDeviceDependentResources();
}

void Graphics::RenderBatch_2D::CreateDeviceDependentResources()
{
	BrushManager::Initialize(m_2DFactory, m_2DDeviceContext, m_wicFactory);
	ThrowIfFailed(
		m_2DDeviceContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&m_defualtBrush)
	);

}

void Graphics::RenderBatch_2D::ReleaseDeviceDependentResources()
{
	m_defualtBrush.Reset();
	m_bitmapRenderTarget.Reset();
	m_textRenderTarget.Reset();
	BrushManager::Reset();
}

void Graphics::RenderBatch_2D::SetDimensions(float width, float height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}

void Graphics::RenderBatch_2D::BeginScene()
{
	// Set tranform on window orientation change
	//m_2DDeviceContext->SetTransform(m_deviceResources->GetOrientationTransform2D());
	m_2DDeviceContext->SaveDrawingState(m_stateBlock.Get());
	m_2DDeviceContext->CreateCompatibleRenderTarget(&m_bitmapRenderTarget);
	m_2DDeviceContext->CreateCompatibleRenderTarget(&m_textRenderTarget);
	m_2DDeviceContext->CreateCompatibleRenderTarget(&m_backgroundRenderTarget);

	m_backgroundRenderTarget->BeginDraw();
	m_textRenderTarget->BeginDraw();
	m_bitmapRenderTarget->BeginDraw();
}

void Graphics::RenderBatch_2D::ClearScreen(float r, float g, float b, float a)
{
	m_backgroundRenderTarget->Clear(D2D1::ColorF(r, g, b, a));
	m_bitmapRenderTarget->Clear(D2D1::ColorF(r, g, b, 0.0f));
	m_textRenderTarget->Clear(D2D1::ColorF(r, g, b, 0.0f));
}

void Graphics::RenderBatch_2D::EndScene()
{
	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	m_backgroundRenderTarget->EndDraw();
	m_textRenderTarget->EndDraw();
	m_bitmapRenderTarget->EndDraw();

	//Begin writing compilation
	m_2DDeviceContext->BeginDraw();
	CreateEffects();
	HRESULT hr = m_2DDeviceContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		ThrowIfFailed(hr);
	}

	m_2DDeviceContext->RestoreDrawingState(m_stateBlock.Get());
}

void Graphics::RenderBatch_2D::CreateEffects()
{
	ComPtr<ID2D1Bitmap> spriteMap;
	ComPtr<ID2D1Bitmap> text;
	ComPtr<ID2D1Bitmap> background;
	auto size = m_2DDeviceContext->GetSize();
	m_bitmapRenderTarget->GetBitmap(&spriteMap);
	m_textRenderTarget->GetBitmap(&text);
	m_backgroundRenderTarget->GetBitmap(&background);

	//Draw Background
	m_2DDeviceContext->DrawBitmap(background.Get(), D2D1::RectF(0.0f, 0.0f, size.width, size.height));

	//Draw sprites
	auto effect = BrushManager::GetEffect();
	if (effect)
	{
		effect->Create(m_2DDeviceContext);
		if (effect->GetBeginEffect() && effect->GetFinalEffect())
		{
			ComPtr<ID2D1Effect> arithmeticCompositeEffect;
			m_2DDeviceContext->CreateEffect(CLSID_D2D1ArithmeticComposite, &arithmeticCompositeEffect);

			effect->GetBeginEffect()->SetInput(0, spriteMap.Get());
			arithmeticCompositeEffect->SetInput(0, background.Get());
			arithmeticCompositeEffect->SetInputEffect(1, effect->GetFinalEffect().Get());

			m_2DDeviceContext->DrawImage(arithmeticCompositeEffect.Get());
		}
	}
	else
	{
		//Failsafe
		m_2DDeviceContext->DrawBitmap(spriteMap.Get(), D2D1::RectF(0.0f, 0.0f, size.width, size.height));
	}

	//Draw out layer
	m_2DDeviceContext->DrawBitmap(text.Get(), D2D1::RectF(0.0f, 0.0f, size.width, size.height));
}

void Graphics::RenderBatch_2D::Draw(const Text & t)
{
	auto format = TextStyleLib::GetFormat(t.GetFormatName());
	if (format)
	{
		auto bounds = t.GetBounds();
		ComPtr<IDWriteTextLayout> textLayout;
		ThrowIfFailed(
			m_writeFactory->CreateTextLayout(
				t.c_str(),
				(uint32_t)t.length(),
				format.Get(),
				bounds.x, // Max width of the input text.
				bounds.y, // Max height of the input text.
				&textLayout
			)
		);
		DWRITE_TEXT_METRICS metrics;
		textLayout->GetMetrics(&metrics);

		auto brush = BrushManager::GetBrush(t.GetBrush());
		if (brush == nullptr)
			brush = m_defualtBrush;

		m_textRenderTarget->DrawTextLayout(
			t.GetPosition(),
			textLayout.Get(),
			brush.Get()
		);
	}
}

void Graphics::RenderBatch_2D::Draw(const Square & s, bool background)
{
	BRUSH_TEXTURE_TYPE type = s.GetBrushType();

	switch (type)
	{
	case Graphics::BRUSH_TEXTURE_TYPE::BRUSH_TEXTURE_DEFAULT:
		DrawRect(s, background);
		break;
	case Graphics::BRUSH_TEXTURE_TYPE::BRUSH_TEXTURE_IMAGE_BRUSH:
		DrawBitmapBrush(s, background);
		break;
	case Graphics::BRUSH_TEXTURE_TYPE::BRUSH_TEXTURE_IMAGE:
		DrawBitmap(s, background);
		break;
	default:
		break;
	}
}

void Graphics::RenderBatch_2D::Draw(const Line & l)
{
	auto brush = BrushManager::GetBrush(l.GetBrush());
	if (brush == nullptr)
		brush = m_defualtBrush;

	m_bitmapRenderTarget->DrawLine(
		l.GetPointOne(),
		l.GetPointTwo(),
		brush.Get(),
		l.GetStrokeWidth()
	);
}

void Graphics::RenderBatch_2D::DrawBitmap(const Square & s, bool background)
{
	int ID = s.GetBrush();
	auto bmp = BrushManager::GetImage(ID);
	if (bmp != nullptr)
	{
		auto rect = s.GetRect();
		auto source = s.GetSourceRect();
		float opacity = s.GetColor().w;
		if (!background)
			m_bitmapRenderTarget->DrawBitmap(bmp.Get(), rect, opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, source);

		m_backgroundRenderTarget->DrawBitmap(bmp.Get(), rect, opacity,
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, source);
	}
	else
		DrawRect(s, background, true);
}

void Graphics::RenderBatch_2D::DrawBitmapBrush(const Square & s, bool background)
{
	int ID = s.GetBrush();
	auto brush = BrushManager::GetImageBrush(ID);

	if (brush != nullptr)
	{
		if (!background)
			m_bitmapRenderTarget->FillRectangle(s.GetRect(), brush.Get());

		m_backgroundRenderTarget->FillRectangle(s.GetRect(), brush.Get());

	}
	else
		DrawRect(s, background, true);
}

void Graphics::RenderBatch_2D::DrawRect(const Square & s, bool background, bool defaultBrush)
{
	ComPtr<ID2D1SolidColorBrush> brush = nullptr;
	if (!defaultBrush)
	{
		int ID = s.GetBrush();
		brush = BrushManager::GetBrush(ID);
	}
	if (brush == nullptr)
		brush = m_defualtBrush;

	if(!background)
		m_bitmapRenderTarget->FillRectangle(s.GetRect(), brush.Get());

	m_backgroundRenderTarget->FillRectangle(s.GetRect(), brush.Get());
		
}
