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
			&m_defualtBrush)
	);

	for (auto& brush : m_solidBrushes)
		brush.second.Reset();
	m_solidBrushes.clear();

	for (auto& colors : m_brushColors)
		CreateNewBrush(colors.first, colors.second, false);

}

void Graphics::RenderBatch_2D::ReleaseDeviceDependentResources()
{
	m_defualtBrush.Reset();
	for (auto& brush : m_solidBrushes)
		brush.second.Reset();
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

		auto& brush = GetBrush(t.GetBrushName());
		m_2DDeviceContext->DrawTextLayout(
			t.GetPosition(),
			textLayout.Get(),
			brush.Get()
		);
	}
}

void Graphics::RenderBatch_2D::Draw(const Square & s)
{
	auto& brush = GetBrush(s.GetBrushName());
	m_2DDeviceContext->FillRectangle(s.GetRectBounds(), brush.Get());
}

void Graphics::RenderBatch_2D::Draw(const Line & l)
{
	auto& brush = GetBrush(l.GetBrushName());
	m_2DDeviceContext->DrawLine(
		l.GetPointOne(),
		l.GetPointTwo(),
		brush.Get(),
		l.GetStrokeWidth()
	);
}

void Graphics::RenderBatch_2D::CreateNewBrush(std::string uniqueName, glm::vec4 color, bool newColor)
{
	auto& found = m_solidBrushes.find(uniqueName);
	if (found != m_solidBrushes.end())
		found->second.Reset();

	if (!newColor)
		m_brushColors[uniqueName] = color;

	m_solidBrushes[uniqueName] = nullptr;
	D2D1_COLOR_F brush;
	brush.r = color.r;
	brush.g = color.g;
	brush.b = color.b;
	brush.a = color.a;
	ThrowIfFailed(
		m_2DDeviceContext->CreateSolidColorBrush(
			brush,
			&m_solidBrushes[uniqueName]
		)
	);
}

void Graphics::RenderBatch_2D::DeleteBrush(std::string uniqueName)
{
	auto& found = m_solidBrushes.find(uniqueName);
	if (found != m_solidBrushes.end())
		found->second.Reset();

	m_solidBrushes.erase(uniqueName);
}

Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> Graphics::RenderBatch_2D::GetBrush(std::string uniqueName)
{
	auto& found = m_solidBrushes.find(uniqueName);
	if (found != m_solidBrushes.end())
	{
		return found->second;
	}
	return m_defualtBrush;
}
