#include "Text.h"

using namespace Microsoft::WRL;

IDWriteFactory3* Graphics::TextStyleLib::m_factory;
std::map<std::string, Microsoft::WRL::ComPtr<IDWriteTextFormat2>> Graphics::TextStyleLib::m_formats;

Graphics::Text::Text()
{
	SetPosition(0.0f, 0.0f);
	SetBrush(-1);
}

Graphics::Text::Text(std::wstring str, std::string style):
	Text()
{
	this->insert(this->begin(), str.begin(), str.end());
	SetFormat(style);
	SetBrush(-1);
}

Graphics::Text::Text(std::string str, std::string style):
	Text()
{
	this->insert(this->begin(), str.begin(), str.end());
	SetFormat(style);
	SetBrush(-1);
}

void Graphics::Text::SetFormat(std::string style)
{
	m_style = style;
}

void Graphics::Text::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void Graphics::Text::SetPosition(glm::vec2 pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
}

void Graphics::Text::SetString(std::string val)
{
	clear();
	insert(this->begin(), val.begin(), val.end());
}

void Graphics::Text::SetString(std::wstring val)
{
	clear();
	insert(this->begin(), val.begin(), val.end());
}

void Graphics::Text::SetBounds(float width, float height)
{
	m_bounds = { width, height };
}

void Graphics::Text::SetBrush(int ID)
{
	m_brush = ID;
}

std::string Graphics::Text::GetFormatName()const
{
	return m_style;
}

D2D_POINT_2F Graphics::Text::GetPosition()const
{
	return m_position;
}

glm::vec2 Graphics::Text::GetBounds()const
{
	return m_bounds;
}

int Graphics::Text::GetBrush() const
{
	return m_brush;
}

void Graphics::Text::operator=(std::string str)
{
	this->SetString(str);
}

void Graphics::Text::operator=(std::wstring str)
{
	this->SetString(str);
}

bool Graphics::TextStyleLib::Initialize(IDWriteFactory3 * writeFactory)
{
	if (writeFactory)
	{
		m_factory = writeFactory;
		TextFormat t;
		t.FontFamilyName = L"Imprint MT Shadow";
		t.FontSize = 20.0f;
		return CreateNewFormat("Default", t);
	}
	return false;
}

void Graphics::TextStyleLib::Close()
{
	ClearFormats();
}

bool Graphics::TextStyleLib::CreateNewFormat(std::string uniqueName, TextFormat format)
{
	auto& found = m_formats.find(uniqueName);
	if (found == m_formats.end())
	{
		ComPtr<IDWriteTextFormat> textFormat;
		ThrowIfFailed(
			m_factory->CreateTextFormat(
				format.FontFamilyName.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_LIGHT,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				format.FontSize,
				L"en-US",
				&textFormat
			)
		);

		ThrowIfFailed(
			textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
		);

		ThrowIfFailed(
			textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)
		);

		m_formats[uniqueName] = nullptr;
		ThrowIfFailed(
			textFormat.As(&m_formats[uniqueName])
		);

		return true;
	}
	return false;
}

void Graphics::TextStyleLib::ClearFormats()
{
	for (auto& x : m_formats)
	{
		x.second.Reset();
	}
	m_formats.clear();
}

const Microsoft::WRL::ComPtr<IDWriteTextFormat2> Graphics::TextStyleLib::GetFormat(std::string uniqueName)
{
	auto& found = m_formats.find(uniqueName);
	if (found != m_formats.end())
		return found->second;

	return m_formats["Default"];
}