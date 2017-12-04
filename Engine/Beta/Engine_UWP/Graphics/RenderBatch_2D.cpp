#include "RenderBatch_2D.h"

using namespace Microsoft::WRL;


Graphics::RenderBatch_2D::RenderBatch_2D(IDWriteFactory3 * writeFactory, ID2D1Factory3 * factory_2D, ID2D1DeviceContext * deviceContext_2D)
{
	m_writeFactory = writeFactory;
	m_2DFactory = factory_2D;
	m_2DDeviceContext = deviceContext_2D;
}

void Graphics::RenderBatch_2D::Initialize()
{
	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

	// Create device independent resources
	ComPtr<IDWriteTextFormat> textFormat;
	ThrowIfFailed(
		m_writeFactory->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			32.0f,
			L"en-US",
			&textFormat
		)
	);

	ThrowIfFailed(
		textFormat.As(&m_textFormat)
	);

	ThrowIfFailed(
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
	);

	ThrowIfFailed(
		m_2DFactory->CreateDrawingStateBlock(&m_stateBlock)
	);

	CreateDeviceDependentResources();
}

void Graphics::RenderBatch_2D::CreateDeviceDependentResources()
{
	ThrowIfFailed(
		m_2DDeviceContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&m_whiteBrush)
	);

	ThrowIfFailed(
		m_2DDeviceContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black, 0.5f),
			&m_blackBrush
		)
	);
}

void Graphics::RenderBatch_2D::ReleaseDeviceDependentResources()
{
	m_whiteBrush.Reset();
}

void Graphics::RenderBatch_2D::SetDimensions(float width, float height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}

void Graphics::RenderBatch_2D::BeginScene()
{
	m_2DDeviceContext->SaveDrawingState(m_stateBlock.Get());
	m_2DDeviceContext->BeginDraw();
}

void Graphics::RenderBatch_2D::EndScene()
{
	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = m_2DDeviceContext->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		ThrowIfFailed(hr);
	}

	m_2DDeviceContext->RestoreDrawingState(m_stateBlock.Get());
}

void Graphics::RenderBatch_2D::Draw(const Text & t)
{
	ComPtr<IDWriteTextLayout> textLayout;
	ThrowIfFailed(
		m_writeFactory->CreateTextLayout(
			t.c_str(),
			(uint32_t)t.length(),
			m_textFormat.Get(),
			240.0f, // Max width of the input text.
			50.0f, // Max height of the input text.
			&textLayout
		)
	);
	DWRITE_TEXT_METRICS metrics;
	textLayout->GetMetrics(&metrics);

	ThrowIfFailed(
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
	);
	//m_2DDeviceContext->SetTransform(m_deviceResources->GetOrientationTransform2D());
	m_2DDeviceContext->SetTransform(t.GetTranslation());
	m_2DDeviceContext->DrawTextLayout(
		D2D1::Point2F(0.f, 0.f),
		textLayout.Get(),
		m_whiteBrush.Get()
	);
}

void Graphics::RenderBatch_2D::Render()
{
	ThrowIfFailed(
		m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
	);

	// Draw a grid background.
	int width = static_cast<int>(m_windowWidth);
	int height = static_cast<int>(m_windowHeight);

	for (int x = 0; x < width; x += 10)
	{
		m_2DDeviceContext->DrawLine(
			D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
			D2D1::Point2F(static_cast<FLOAT>(x), m_windowHeight),
			m_whiteBrush.Get(),
			0.5f
		);
	}

	for (int y = 0; y < height; y += 10)
	{
		m_2DDeviceContext->DrawLine(
			D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
			D2D1::Point2F(m_windowWidth, static_cast<FLOAT>(y)),
			m_whiteBrush.Get(),
			0.5f
		);
	}

	m_2DDeviceContext->DrawTextLayout(
		D2D1::Point2F(0.f, 0.f),
		m_textLayout.Get(),
		m_whiteBrush.Get()
	);

	D2D1_RECT_F rectangle1 = D2D1::RectF(
		m_windowWidth / 2 - 50.0f,
		m_windowHeight / 2 - 50.0f,
		m_windowWidth / 2 + 50.0f,
		m_windowHeight / 2 + 50.0f
	);

	m_2DDeviceContext->FillRectangle(rectangle1, m_blackBrush.Get());
}
