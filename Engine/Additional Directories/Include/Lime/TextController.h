#pragma once
#include "Shared.h"
#include "Model3D.h"

namespace Lime
{
	class TextInfo
	{
		friend class DX11Graphics;
		friend class TextController;
	public:
		AppDLL_API TextInfo(std::string str);
		AppDLL_API void SetText(std::string text);
		AppDLL_API const std::string GetText() const;
		AppDLL_API float GetTextOffset();

	protected:
		std::shared_ptr<Model::Model3D>& GetMesh();

	private:
		static MeshID LoadModel();

	protected:
		float middleX = 0.0f;
		float middleY = 0.0f;
		std::string text;
		std::shared_ptr<Model::Model3D> data;
	};

	class TextController
	{
	public:
		AppDLL_API TextController(std::string str);
		AppDLL_API void Color(glm::vec4 two);
		AppDLL_API void Position(glm::vec3 position);
		AppDLL_API void Scale(glm::vec3 scale);
		AppDLL_API void SetText(std::string text);
		AppDLL_API const std::shared_ptr<TextInfo>& GetInfo();
	private:

		std::shared_ptr<TextInfo> info;
	};

}