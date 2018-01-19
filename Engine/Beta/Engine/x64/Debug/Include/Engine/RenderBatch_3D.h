#pragma once
#include "Camera.h"
#include "ShaderLibrary.h"
#include "BufferLibrary.h"
#include "RSSLibrary.h"
#include "TextureLibrary.h"
#include "VertexManager.h"
#include "MeshLoader.h"

//Macros
#define FLAG_SIZE 4
#define MAX_INSTANCES 500

namespace Engine
{
	struct PBInfo
	{
		uint32_t flags[FLAG_SIZE];
		glm::mat4 view;
		glm::mat4 projection;
		glm::vec4 camera;
		PerInstanceInfo instances[MAX_INSTANCES];
	};

	class RenderBatch_3D
	{
	public:
		RenderBatch_3D(ID3D11Device3* device, ID3D11DeviceContext3* context);
		void Reset();
		bool Initialize(std::shared_ptr<Camera>& camera);
		void Draw(const Model& model);
		void Wireframe(bool value);
		void ProcessScene();
		const std::shared_ptr<TextureLibrary>& GetTextureLibrary();
	private:
		void ProcessObjects(Batch & batch);

		//Rendering
		void ProcessObject_3DTriangles(BatchInfo & info);

		bool FillBuffers();
		void CreateShaders();
		void CreateConstBuffers();
		bool CreateRSSStates();

		//New API
		bool m_bWireframe;
		ID3D11Device3* m_device;
		ID3D11DeviceContext3* m_context;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<TextureLibrary> m_textureLib;
		RSSLibrary m_rssLib;
		ShaderLibrary m_shaderLib;
		BufferLibrary m_bufferLib;
		VertexManager m_vertexManager;
	};


}