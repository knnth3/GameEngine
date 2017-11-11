#pragma once
#include "Camera.h"
#include "DllSettings.h"
#include "DX11DepthStencilState.h"
#include "DX11BufferManager.h"
#include "VertexManager.h"
#include "DX11ShaderManager.h"
#include "DX11RasterStateManager.h"

namespace Graphics
{
	struct MatrixBuffer
	{
		glm::mat4 world;
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec4 color;
		glm::vec3 cameraPos;
		float padding;
	};

	struct Float8
	{
		glm::vec4 one;
		glm::vec4 two;
	};
	struct PF_PixelBuffer
	{
		glm::vec4 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		glm::vec3 lightDirection;
		float specularPower;
	};

	class DX11Graphics
	{
	public:
		DX11Graphics(std::shared_ptr<DX11BufferManager>& bufferManager, std::shared_ptr<DX11ShaderManager> shaderManager, 
			std::shared_ptr<DX11DepthStencilState>& depthStencilState,
			std::shared_ptr<DX11RasterStateManager>& rasterStateManager);
		~DX11Graphics();
		void Reset();
		bool Initialize(std::shared_ptr<Camera>& camera);
		void AddModel(std::shared_ptr<Model>& model);
		void Draw();

	private:

		//3D Rendering
		void ProcessObject_3DTriangles(VertexManagerInfo & info);
		void ProcessObject_3DPointList(VertexManagerInfo & info);
		void ProcessObject_3DLineList(VertexManagerInfo & info);

		//2D Rendering
		void ProcessObject_2DTriangles(VertexManagerInfo & info);
		void ProcessObject_2DPointList(VertexManagerInfo & info);
		void ProcessObject_2DLineList(VertexManagerInfo & info);

		void FillBuffers();
		void CreateShaders();
		void CreateConstBuffers();
		bool CreateRSSStates();
		bool SetRSStyleActive(RSS_STYLES style);
		void SetActiveTexture(TextureID id);

		//New API
		bool m_bFilled;
		const uint32_t m_bufferCount;
		WorldLight m_light;

		//3D Processing
		std::vector<int> m_modelDrawCache_3D;
		std::map<int, VertexManagerInfo> m_cachedModels_3D;
		std::shared_ptr<VertexManager> m_vertexManager;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<DX11BufferManager> m_bufferManager;
		std::shared_ptr<DX11ShaderManager> m_shaderManager;
		std::shared_ptr<DX11DepthStencilState> m_depthStencilState;
		std::shared_ptr<DX11RasterStateManager> m_RSSManager;

	};


}