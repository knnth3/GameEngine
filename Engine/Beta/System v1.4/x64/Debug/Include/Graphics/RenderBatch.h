#pragma once
#include "Camera.h"
#include "DllSettings.h"
#include "DX11DepthStencilState.h"
#include "DX11BufferManager.h"
#include "VertexManager.h"
#include "DX11ShaderManager.h"
#include "DX11RasterStateManager.h"

//Macros
#define FLAG_SIZE 4
#define MAX_INSTANCES 500

namespace Graphics
{
	struct PBInfo
	{
		uint32_t flags[FLAG_SIZE];
		glm::mat4 view;
		glm::mat4 projection;
		PerInstanceInfo instances[MAX_INSTANCES];
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
		void AddModel(Model& model);
		void DrawBackToFront(bool value);
		void Draw();

	private:
		void ProcessObjects(Batch & batch);

		//Rendering
		void ProcessObject_3DTriangles(BatchInfo & info);
		void ProcessObject_2DTriangles(BatchInfo & info);

		bool FillBuffers();
		void CreateShaders();
		void CreateConstBuffers();
		bool CreateRSSStates();
		bool SetRSStyleActive(RSS_STYLES style);
		void SetActiveTexture(TextureID id);

		//New API
		bool m_bDrawReversed;
		std::shared_ptr<VertexManager> m_vertexManager;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<DX11BufferManager> m_bufferManager;
		std::shared_ptr<DX11ShaderManager> m_shaderManager;
		std::shared_ptr<DX11DepthStencilState> m_depthStencilState;
		std::shared_ptr<DX11RasterStateManager> m_RSSManager;

	};


}