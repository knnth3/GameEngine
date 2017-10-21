#pragma once
#include "RenderBatch.h"

namespace Lime
{
	struct PIXELFORMATDESCRIPTOR_1
	{
		int size;
		int version;
		int formatSupport;
		int typeRGBA;
		int bits;
		int colorBit1, colorBit2, colorBit3, colorBit4, colorBit5, colorBit6;
		int alphaBuffer;
		int shiftBit;
		int accumulationBuffer;
		int accumulationBit1, accumulationBit2, accumulationBit3, accumulationBit4;
		int sixteenBitBuffer;
		int stencilBuffer;
		int auxiliararyBuffer;
		int mainPlane;
		int reserved;
		int layerMask1, layerMask2, layerMask3;

	};


	class OpenGLGraphics :
		public RenderBatch
	{
	public:
		AppDLL_API OpenGLGraphics(const void* window, const uint32_t width, const uint32_t height);
		AppDLL_API ~OpenGLGraphics();
		AppDLL_API bool Add3DModel(std::shared_ptr<Model::Model3D>& model) override;
		AppDLL_API bool Add2DModel(std::shared_ptr<Model::Model2D>& model) override;
		AppDLL_API bool Add3DLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec4 color) override;
		AppDLL_API bool AddText(std::string text, std::shared_ptr<TextController>& controller) override;
		AppDLL_API void AttatchCamera(std::shared_ptr<Camera>& ptr) override;
		AppDLL_API void Draw() override;
		AppDLL_API void ResizeWindow(const uint32_t width, const uint32_t height) override;
		AppDLL_API void Wireframe(bool statement) override;
		AppDLL_API void ClearScreen(glm::vec3 color) override;
		AppDLL_API void Reset() override;

	};

}