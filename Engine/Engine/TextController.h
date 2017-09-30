#pragma once
#include "Primitives.h"


namespace Lime
{
	class TextInfo
	{
		friend class TextController;
	public:
		DLL_EXPORT TextInfo(std::string str);
		DLL_EXPORT const std::shared_ptr<Model2>& GetData();
		DLL_EXPORT void SetText(std::string text);
		DLL_EXPORT const std::string GetText() const;
		DLL_EXPORT float GetTextOffset();

	private:
		static void LoadModel(std::shared_ptr<ModelData2>& info);

	protected:
		float middleX = 0.0f;
		float middleY = 0.0f;
		std::string text;
		std::shared_ptr<Model2> data;
	};

	class TextController
	{
	public:
		DLL_EXPORT TextController(std::string str);
		DLL_EXPORT void Color(glm::vec4 color);
		DLL_EXPORT void Position(glm::vec3 position);
		DLL_EXPORT void Scale(glm::vec3 scale);
		DLL_EXPORT void SetText(std::string text);
		DLL_EXPORT const std::shared_ptr<TextInfo>& GetInfo();
	private:

		std::shared_ptr<TextInfo> info;
	};

}