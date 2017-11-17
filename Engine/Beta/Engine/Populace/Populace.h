#pragma once
#include <string>
#include "Biosphere.h"
#include "DLLSettings.h"



PL_DLL_API inline bool PL_Initialize(std::string directory);
PL_DLL_API inline void PL_Close();
PL_DLL_API inline PL_Actor_ID PL_CreateActor(std::string name);
PL_DLL_API inline bool PL_GetActorData(const PL_Actor_ID id, PL::PL_ActorData& data);
PL_DLL_API inline void PL_WriteToDisk();


namespace PL
{
	PL_DLL_API inline void Update_Biosphere();
	static Biosphere* m_biosphere = nullptr;
	static std::thread m_thread;
}