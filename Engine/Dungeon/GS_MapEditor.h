#pragma once
#include "GameState.h"
#include <Lime\DX11Graphics.h>

//Game state for Level editor
//Uses pre made models to tile map
//The character model is not my own. By 3dregenerator.
//Downloaded from https://free3d.com/download-page.php?url=puo-3971-61889




namespace GameStates
{
	class MapEditor :
		public GameState
	{
	public:

		MapEditor(std::shared_ptr<Lime::DX11Graphics>& rend, std::shared_ptr<Lime::Camera>& camera);

		//Function called every frame
		virtual States Update(float time, std::shared_ptr<Lime::InputManager>& input) override;

		//State Changes
		virtual void Initialize(int windowWidth, int windowHeight) override;
		virtual void Close() override;
		virtual void OnWindowResize(int width, int height) override;


		void Draw();

	private:
		void CreateFloor(int length, int width, Lime::TextureID id, std::shared_ptr<Lime::DX11Graphics>& rend);
		void UpdateCursor(float time, std::shared_ptr<Lime::InputManager>& input);
		void AddNewBlock(std::shared_ptr<Lime::Model::Model3D>& model);

		float camRot = 0.0f;
		std::shared_ptr<Lime::Model::Model3D> m_user;
		std::shared_ptr<Lime::Model::Model3D> m_cursor;
		std::shared_ptr<Lime::Camera> m_camera;

		std::vector<std::vector<std::shared_ptr<Lime::Model::Model3D>>> m_floor;
		std::vector<std::shared_ptr<Lime::Model::Model3D>> m_newBlocks;
		std::shared_ptr<Lime::DX11Graphics> m_render;
	};

}
