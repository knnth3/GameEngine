#include "TextController.h"

Lime::TextInfo::TextInfo(std::string str)
{
	data = std::make_shared<Model2>();
	data->m_ptr = this;
	TextInfo::LoadModel(data->m_Data);
	SetText(str);
}

const std::shared_ptr<Lime::Model2>& Lime::TextInfo::GetData()
{
	return data;
}

void Lime::TextInfo::SetText(std::string str)
{
	text = str;
	float offset = GetTextOffset();
	data->SetOffset(offset);
}

const std::string Lime::TextInfo::GetText() const
{
	return text;
}

float Lime::TextInfo::GetTextOffset()
{
	int textLen = text.length() - 1;
	float totalWidth = 1.2 * textLen;
	float totalHeight = 2.0f;
	float middleX = totalWidth / 2.0f;
	float middleY = totalHeight / 2.0f;
	return middleX;
}

void Lime::TextInfo::LoadModel(std::shared_ptr<ModelData2>& info)
{
	static bool isFirst = true;
	static auto data = std::make_shared<ModelData2>();
	if (isFirst)
	{
		data->renderType = "Text";
		data->m_Verticies =
		{
			// Front Face
			Vertex2(-0.6f, -1.0f, -1.0f, 0.0f, 1.0f),
			Vertex2(-0.6f,  1.0f, -1.0f, 0.0f, 0.0f),
			Vertex2(0.6f,  1.0f, -1.0f,  1.0f, 0.0f),
			Vertex2(0.6f, -1.0f, -1.0f,  1.0f, 1.0f),
		};

		data->m_Indicies = {
			// Front Face
			0,  1,  2,
			0,  2,  3,
		};
	}
	info = data;
}

Lime::TextController::TextController(std::string str)
{
	info = std::make_shared<TextInfo>(str);
}

void Lime::TextController::Color(glm::vec4 color)
{
	info->data->Color(color.r, color.g, color.b, color.a);
}

void Lime::TextController::Position(glm::vec3 position)
{
	info->data->Translate(position);
}

void Lime::TextController::Scale(glm::vec3 scale)
{
	info->data->Scale(scale.x, scale.y, scale.z);
}

void Lime::TextController::SetText(std::string text)
{
	info->SetText(text);
}

const std::shared_ptr<Lime::TextInfo>& Lime::TextController::GetInfo()
{
	return info;
}
