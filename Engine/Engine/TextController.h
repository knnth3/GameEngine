#pragma once
#include "Primitives.h"


namespace Lime
{
	class TextInfo
	{
		friend class TextController;
	public:
		AppDLL_API TextInfo(std::string str);
		AppDLL_API const std::shared_ptr<Model3D>& GetData();
		AppDLL_API void SetText(std::string text);
		AppDLL_API const std::string GetText() const;
		AppDLL_API float GetTextOffset();

	private:
		static void LoadModel(std::shared_ptr<ModelData>& info);

	protected:
		float middleX = 0.0f;
		float middleY = 0.0f;
		std::string text;
		std::shared_ptr<Model3D> data;
	};

	class TextController
	{
	public:
		AppDLL_API TextController(std::string str);
		AppDLL_API void Color(glm::vec4 color);
		AppDLL_API void Position(glm::vec3 position);
		AppDLL_API void Scale(glm::vec3 scale);
		AppDLL_API void SetText(std::string text);
		AppDLL_API const std::shared_ptr<TextInfo>& GetInfo();
	private:

		std::shared_ptr<TextInfo> info;
	};

}