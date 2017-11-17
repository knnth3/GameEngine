#pragma once
#include <vector>
#include <mutex>
#include <atomic>
#include "Actor.h"
#define MAX_ACTORS_PER_SCENE 100
#define ACTOR_ID_NONVALID -1


typedef int16_t PL_Actor_ID;

namespace PL
{
	struct PL_ActorData
	{

		std::string Name;
		int16_t MaxHealth;
		int16_t CurrentHealth;
	};

	class Biosphere
	{
	public:
		Biosphere(std::string Folder);

		//Access functions
		PL_Actor_ID SpawnActor(const std::string name);
		bool GetActor(const PL_Actor_ID id, PL_ActorData& data);

		//Update
		void Update();
		void Close();

		//Save
		void WriteToDisk();


	private:

		const std::string m_folder;
		std::atomic_bool m_bClose;
		std::atomic_bool m_bSave;
		std::mutex m_threadLock;
		std::vector<PL_Actor> m_actors;

	};
}