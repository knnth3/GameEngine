#pragma once
#include <string>

namespace PL
{
	class PL_Actor
	{
	public:
		PL_Actor(std::string name);

		//Get Functions
		std::string GetName();
		int16_t GetCurrentHealth();
		int16_t GetMaxHealth();
	private:

		std::string m_name;
		int16_t m_maxHealth;
		int16_t m_currentHealth;

	};
}