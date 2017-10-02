#include "TextController.h"

Lime::TextInfo::TextInfo(std::string str)
{
	data = std::make_shared<Model3D>();
	data->m_ptr = this;
	TextInfo::LoadModel(data->m_Data);
	SetText(str);
}

const std::shared_ptr<Lime::Model3D>& Lime::TextInfo::GetData()
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
	int textLen = (int)text.length() - 1;
	float totalWidth = 1.2f * textLen;
	float totalHeight = 2.0f;
	float middleX = totalWidth / 2.0f;
	float middleY = totalHeight / 2.0f;
	return middleX;
}

void Lime::TextInfo::LoadModel(std::shared_ptr<ModelData>& info)
{
	static bool isFirst = true;
	static auto data = std::make_shared<ModelData>();
	if (isFirst)
	{
		glm::vec3 normal(0.0f, 0.0f, 1.0f);
		data->renderType = "Text";
		data->m_Verticies =
		{
			// Front Face

			Vertex(glm::vec3(-0.6f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f), normal),
			Vertex(glm::vec3(-0.6f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), normal),
			Vertex(glm::vec3(0.6f,  1.0f, -1.0f), glm::vec2(1.0f, 0.0f), normal),
			Vertex(glm::vec3(0.6f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f), normal),
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
