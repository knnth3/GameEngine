#pragma once
#include "PL_Item.h"

namespace PL
{
	class PL_Consumable:
		public PL_Item
	{
	public:
		PL_Consumable();
		PL_Consumable(bool cooked, int tasteLevel, int healthBonus);

		//Set Functions
		void SetSaying(std::string saying);
		void SetNumPortions(unsigned int portions);

		//Get Functions
		std::string GetSaying();
		unsigned int GetNumPortions();

		//User Functions
		bool ConsumePortion(unsigned int portions);
		bool Consumed();

	private:

		bool m_bIsCooked;
		int m_tasteLevel;
		int m_healthBonus;
		unsigned int m_portions;
		std::string m_saying;
	};

}