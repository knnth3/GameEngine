#include "Text.h"

using namespace Microsoft::WRL;

IDWriteFactory3* Engine::TextStyleLib::m_factory;
std::map<std::string, Microsoft::WRL::ComPtr<IDWriteTextFormat2>> Engine::TextStyleLib::m_formats;

Engine::Text::Text()
{
	SetPosition(0.0f, 0.0f);
	SetBrush(-1);
}

Engine::Text::Text(std::wstring str, std::string style):
	Text()
{
	this->insert(this->begin(), str.begin(), str.end());
	SetFormat(style);
	SetBrush(-1);
}

Engine::Text::Text(std::string str, std::string style):
	Text()
{
	this->insert(this->begin(), str.begin(), str.end());
	SetFormat(style);
	SetBrush(-1);
}

void Engine::Text::SetFormat(std::string style)
{
	m_style = style;
}

void Engine::Text::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void Engine::Text::SetPosition(glm::vec2 pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
}

void Engine::Text::SetString(std::string val)
{
	clear();
	insert(this->begin(), val.begin(), val.end());
}

void Engine::Text::SetString(std::wstring val)
{
	clear();
	insert(this->begin(), val.begin(), val.end());
}

void Engine::Text::SetBounds(float width, float height)
{
	m_bounds = { width, height };
}

void Engine::Text::SetBrush(int ID)
{
	m_brush = ID;
}

std::string Engine::Text::GetFormatName()const
{
	return m_style;
}

D2D_POINT_2F Engine::Text::GetPosition()const
{
	return m_position;
}

glm::vec2 Engine::Text::GetBounds()const
{
	return m_bounds;
}

int Engine::Text::GetBrush() const
{
	return m_brush;
}

void Engine::Text::operator=(std::string str)
{
	this->SetString(str);
}

void Engine::Text::operator=(std::wstring str)
{
	this->SetString(str);
}

bool Engine::TextStyleLib::Initialize(IDWriteFactory3 * writeFactory)
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

void Engine::TextStyleLib::Close()
{
	ClearFormats();
}

bool Engine::TextStyleLib::CreateNewFormat(std::string uniqueName, TextFormat format)
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

void Engine::TextStyleLib::ClearFormats()
{
	for (auto& x : m_formats)
	{
		x.second.Reset();
	}
	m_formats.clear();
}

const Microsoft::WRL::ComPtr<IDWriteTextFormat2> Engine::TextStyleLib::GetFormat(std::string uniqueName)
{
	auto& found = m_formats.find(uniqueName);
	if (found != m_formats.end())
		return found->second;

	return m_formats["Default"];
}