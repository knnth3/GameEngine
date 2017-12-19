#pragma once
#include "Core\GameState.h"



namespace GameStates
{
	class TestingRealm :
		public GameState
	{
	public:

		//State Changes
		virtual bool CreateDeviceIndependentResources()override;
		virtual void CreateDeviceDependentResources()override;
		virtual States Update()override;
		virtual void Draw()override;

		virtual void Close()override;
		virtual void OnWindowResize()override;

	private:

		Graphics::Model m_player;
		Graphics::Model m_floor;
	};

}