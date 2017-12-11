#pragma once
#include "Scene.h"

namespace Scenes
{
	class Scene_Level1 :
		public Scene
	{
	public:
		Scene_Level1(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera);
		~Scene_Level1();

		//Overloaded Functions
		virtual void Initialize() override;
		virtual void Update(float time, std::shared_ptr<Lime::InputManager>& input) override;

	private:
		//Private Funcions
		void Draw();
		void ProcessInput(float time, std::shared_ptr<Lime::InputManager>& input);

		//Private variables
		std::vector<std::shared_ptr<Lime::Model::Model3D>> m_map;

	};

}
