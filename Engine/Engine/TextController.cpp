#include "TextController.h"

Lime::TextInfo::TextInfo(std::string str) :
	TextInfo()
{
	SetText(str);
	data = std::make_shared<Model2>();
	controller = std::make_shared<TextController>(this);
}

void Lime::TextInfo::GetController(std::shared_ptr<TextController>& ptr) const
{
	ptr = controller;
}

void Lime::TextInfo::GetData(std::shared_ptr<Model2>& ptr) const
{
	ptr = data;
}

void Lime::TextInfo::SetText(std::string str)
{
	text = str;
	TextInfo::LoadModel(this);
}

const std::string Lime::TextInfo::GetText() const
{
	return text;
}

float Lime::TextInfo::UpdateMiddlePos()
{
	int textLen = text.length();
	float totalWidth = 1.2 * textLen;
	float totalHeight = 2.0f;
	float middleX = totalWidth / 2.0f;
	float middleY = totalHeight / 2.0f;
	return middleX;
}

void Lime::TextInfo::LoadModel(void * self)
{
	static bool isFirst = true;
	static auto data = std::make_shared<ModelData2>();
	data->renderType = "Text";
	data->m_Verticies =
	{
		// Front Face
		Vertex2(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex2(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex2(1.0f,  1.0f, -1.0f,  1.0f, 0.0f),
		Vertex2(1.0f, -1.0f, -1.0f,  1.0f, 1.0f),
	};

	data->m_Indicies = {
		// Front Face
		0,  1,  2,
		0,  2,  3,
	};
	TextInfo* thisObject = reinterpret_cast<TextInfo*>(self);
	thisObject->data->m_Data = data;
}

Lime::TextController::TextController(TextInfo* i)
{
	info = i;
}

void Lime::TextController::Color(glm::vec4 color)
{
	std::shared_ptr<Model2> data = nullptr;
	info->GetData(data);
	data->Color(color.r, color.g, color.b);
}

void Lime::TextController::Position(glm::vec3 position)
{
	std::shared_ptr<Model2> data = nullptr;
	info->GetData(data);
	data->Translate(position.x, position.y, position.z);
}

void Lime::TextController::Scale(glm::vec3 scale)
{
	std::shared_ptr<Model2> data = nullptr;
	info->GetData(data);
	data->Scale(scale.x, scale.y, scale.z);
}

void Lime::TextController::SetText(std::string text)
{
	info->SetText(text);
}
