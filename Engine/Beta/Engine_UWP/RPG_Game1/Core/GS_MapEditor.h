#pragma once
#include "GameState.h"
#include "Effects\SpotLightDiffuse.h"


namespace GameStates
{
	class MapEditor :
		public GameState
	{
	public:
		/////////////Inherited functions from Gamesate/////////////////////////
		//--If creating a new function make sure to write *override* at the end of the function implementation
		//--This does nothing but it does make it clear that it's inherited
		//--The same applies to the virtual before the function


		virtual bool CreateDeviceIndependentResources()override;
		virtual void CreateDeviceDependentResources()override;
		virtual States Update()override;
		virtual void Draw()override;
		virtual void Close()override;
		virtual void OnWindowResize()override;

		/////////////////////////////////////////////////////////////////////////

	private:

		void UpdateCursor(bool buttonPressed, float elapsed);
		void AddNewBlock(Graphics::Square model);
		void CalculateLighting(glm::vec2 pos);


		Graphics::Square m_cursor;
		Graphics::Square m_floor;
		std::vector<Graphics::Square> m_map;
		std::shared_ptr<Effects::SpotLightDiffuse> m_effect;
		Microsoft::WRL::ComPtr<ID2D1Effect> m_spotDiffuseEffect;
	};
}
