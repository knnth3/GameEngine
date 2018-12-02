#pragma once
#include "universal_map.h"
#include "GraphicsDevice.h"
#include "Timer.h"
#include "Input.h"
#include "Console.h"

namespace Engine
{
	typedef std::shared_ptr<Engine::GraphicsDevice> EGraphics;
	typedef std::shared_ptr<Input> EInput;
	typedef std::shared_ptr<Timer> ETimer;
	typedef std::shared_ptr<Console> EConsole;
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
		__engine_decl static void SetGraphics(const universal_struct& graphics);
		__engine_decl static void SetInput(const universal_struct& input);
		__engine_decl static void SetTimer(const universal_struct& timer);
		__engine_decl static void SetConsole(const universal_struct& console);
		__engine_decl static void RemoveAddon(const std::string& key);
		__engine_decl static bool InsertAddon(const std::string& key, const universal_struct& obj);
		__engine_decl static EGraphics GetGraphics();
		__engine_decl static EInput GetInput();
		__engine_decl static ETimer GetTimer();
		__engine_decl static EConsole GetConsole();
		__engine_decl static const universal_struct* GetAddon(const std::string& key);
		__engine_decl static std::vector<std::string> GetKeyList();

	protected:
		friend class GraphicsWindow;
		__engine_decl static void Reset();

	private:
		static bool IsReserved(const std::string& key);
		static const std::string m_reserved[4];
		static universal_map m_resources;
	};

}

