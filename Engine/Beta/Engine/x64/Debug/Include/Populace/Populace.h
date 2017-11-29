#pragma once
#include <string>
#include "Biosphere.h"
#include "DLLSettings.h"



PL_DLL_API inline bool PL_Initialize(const std::string DomainName);
PL_DLL_API inline void PL_Close();
PL_DLL_API inline bool PL_CreateActor(std::string name);
PL_DLL_API inline bool PL_KillActor(std::string name);
PL_DLL_API inline bool PL_GetActorData(const std::string name, PL_ActorData& data);
PL_DLL_API inline void PL_ClearDeadActors();
PL_DLL_API inline bool PL_GiveItem(const std::string name, const PL_Item_Desc item);
PL_DLL_API inline void PL_Save();


namespace PL
{
	PL_DLL_API inline void Update_Biosphere();
	static Biosphere* m_biosphere = nullptr;
	static std::thread m_thread;
}