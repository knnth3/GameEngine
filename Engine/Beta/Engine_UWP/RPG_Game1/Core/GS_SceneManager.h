#pragma once
#include "GameState.h"
#include "Actor.h"
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

		void UpdateLighting();
		bool LoadScene();
		bool BeginNewScene();
		void ShutDownScene();

		int m_spriteSheetID;
		glm::vec2 m_lightVec;
		std::vector<Graphics::Line> m_pathLines;
		std::vector<Actor> m_actors;
		Graphics::Square m_ground;
		std::shared_ptr<Effects::SpotLightDiffuse> m_effect;
		PathFinder m_pathFinder;
	};


}