#include "TextController.h"
#include "ModelLoader.h"

Lime::TextInfo::TextInfo(std::string str)
{
	data = std::make_shared<Model::Model3D>(TextInfo::LoadModel());
	data->m_meshType = Model::TEXT;
	data->m_ptr = this;
	SetText(str);
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

std::shared_ptr<Lime::Model::Model3D>& Lime::TextInfo::GetMesh()
{
	return data;
}

MeshID Lime::TextInfo::LoadModel()
{
	static bool isFirst = true;
	static auto data = Model::MeshLoader::LoadModel("EngineAssets/models/TextBox.fbx");
	return data;
}

Lime::TextController::TextController(std::string str)
{
	info = std::make_shared<TextInfo>(str);
}

void Lime::TextController::Color(glm::vec4 color)
{
	info->data->SetColor(color.r, color.g, color.b, color.a);
}

void Lime::TextController::Position(glm::vec3 position)
{
	info->data->SetPosition(position);
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
