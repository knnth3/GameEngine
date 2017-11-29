#pragma once
#include <vector>
#include <mutex>
#include <atomic>
#include "Actor.h"


namespace PL
{

	class Biosphere
	{
	public:
		Biosphere(std::string Folder);

		//Access functions
		bool SpawnActor(const std::string name);
		bool GetActor(const std::string name, PL_ActorData& data);

		//Update
		void Update();
		void Close();
		bool KillActor(const std::string name);
		void ClearDeadActors();
		bool GiveItem(const std::string name, const PL_Item_Desc item);

		//Save
		void Save();


	private:
		void ClearDead();
		void WriteToDisk();

		const std::string m_folder;
		std::atomic_bool m_bClose;
		std::atomic_bool m_bSave;
		std::atomic_bool m_bClearDead;
		std::mutex m_threadLock;

	};
}