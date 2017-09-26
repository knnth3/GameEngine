#pragma once
#include "Primitives.h"


namespace Lime
{
	class TextInfo;

	class TextController
	{
	public:
		DLL_EXPORT TextController(TextInfo* i);
		DLL_EXPORT void Color(glm::vec4 color);
		DLL_EXPORT void Position(glm::vec3 position);
		DLL_EXPORT void Scale(glm::vec3 scale);
		DLL_EXPORT void SetText(std::string text);

	private:

		TextInfo* info;
	};

	class TextInfo
	{
	public:
		DLL_EXPORT TextInfo(std::string str);
		DLL_EXPORT void GetController(std::shared_ptr<TextController>& ptr) const;
		DLL_EXPORT void GetData(std::shared_ptr<Model2>& ptr) const;
		DLL_EXPORT void SetText(std::string text);
		DLL_EXPORT const std::string GetText() const;
		DLL_EXPORT float UpdateMiddlePos();
	private:
		static void LoadModel(void* self);
		TextInfo();

		float middleX = 0.0f;
		float middleY = 0.0f;
		std::string text;
		std::shared_ptr<Model2> data;
		std::shared_ptr<TextController> controller;
	};


}