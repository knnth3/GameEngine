#pragma once
#include "GameState.h"
#include "Character.h"
#include "Item.h"
#include "Effects\SpotLightDiffuse.h"
#include "PathFinder.h"

//A scene represents a level
//Levels will be loaded and simulated in this implementation

struct Scene
{

};

namespace GameStates
{
	class SceneManager :
		public GameState
	{
	public:
		virtual bool CreateDeviceIndependentResources()override;
		virtual void CreateDeviceDependentResources()override;
		virtual void Close()override;
		virtual void Draw()override;
		virtual States Update()override;
		virtual void OnWindowResize()override;

	private:
		bool CollisionDetect(Actor* m_actor);
		void UpdateLighting();
		bool LoadScene();
		bool BeginNewScene();
		void ShutDownScene();

		int m_spriteSheetID;
		glm::vec2 m_lightVec;
		std::vector<Graphics::Line> m_pathLines;
		std::vector<Character> m_actors;
		std::vector<Item> m_items;
		Graphics::Square m_ground;
		std::shared_ptr<Effects::SpotLightDiffuse> m_effect;
		PathFinder m_pathFinder;
	};


}