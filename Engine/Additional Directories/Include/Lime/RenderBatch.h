#pragma once

#include "Camera.h"
#include "TextController.h"
#include "Shared.h"
#include "DX11Shader.h"
#include "DX11DepthStencilState.h"
#include "DX11BufferManager.h"
#include "VertexLibrary.h"

namespace Lime
{
	class RenderBatch
	{
	public:
		virtual bool Add3DModel(std::shared_ptr<Model::Model3D>& model) = 0;
		virtual bool Add2DModel(std::shared_ptr<Model::Model2D>& model) = 0;
		virtual bool Add3DLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color) = 0;
		virtual bool AddText(std::string text, std::shared_ptr<TextController>& controller) = 0;
		virtual void AttatchCamera(std::shared_ptr<Camera>& ptr) = 0;
		virtual void Draw() = 0;
		virtual void ResizeWindow(const uint32_t width, const uint32_t height) = 0;
		virtual void Wireframe(bool statement) = 0;
		virtual void ClearScreen(glm::vec3 color) = 0;
		virtual void Reset() = 0;
	};
}

