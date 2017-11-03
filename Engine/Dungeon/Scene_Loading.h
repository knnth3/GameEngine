#pragma once
#include "Scene.h"

namespace Scenes
{
	class Scene_Loading :
		public Scene
	{
	public:
		Scene_Loading(std::shared_ptr<Lime::RenderBatch>& rend, std::shared_ptr<Lime::Camera>& camera);
		~Scene_Loading();

		//Overloaded Functions
		virtual void Initialize() override;
		virtual void Update(float time, std::shared_ptr<Lime::InputManager>& input) override;

	private:

	};
}

