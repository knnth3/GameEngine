#pragma once
#include "universal_map.h"
#include "Graphics.h"
#include "Timer.h"
#include "Input.h"

namespace Graphics
{
	/*Holds all data/objects relevant to the active window.
	Data is stored as universal_structs and must be cast to use.
	Because objets passed to this interface become "global", ensure that they are thread safe!
	Helper functions will not work if given graphics interfaces do not match expected output types.
	Ex: GetInput() etc. can still be obtained through GetAddon() function using designated reserved keys.
	Reserved keys:
	-GRAPHICS
	-INPUT
	-TIMER*/
	class WindowResources
	{
	public:
		GRAPHICS_DLL_API static void SetGraphics(const universal_struct& graphics);
		GRAPHICS_DLL_API static void SetInput(const universal_struct& input);
		GRAPHICS_DLL_API static void SetTimer(const universal_struct& timer);
		GRAPHICS_DLL_API static void RemoveAddon(const std::string& key);
		GRAPHICS_DLL_API static bool InsertAddon(const std::string& key, const universal_struct& obj);
		GRAPHICS_DLL_API static std::shared_ptr<GraphicsDevice> GetGraphics();
		GRAPHICS_DLL_API static std::shared_ptr<Input> GetInput();
		GRAPHICS_DLL_API static std::shared_ptr<Timer> GetTimer();
		GRAPHICS_DLL_API static const universal_struct* GetAddon(const std::string& key);
		GRAPHICS_DLL_API static std::vector<std::string> GetKeyList();

	protected:
		friend class GraphicsWindow;
		GRAPHICS_DLL_API static void Reset();

	private:
		static bool IsReserved(const std::string& key);
		static const std::string m_reserved[3];
		static universal_map m_resources;
	};

}

