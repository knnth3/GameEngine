#pragma once
#include "DllSettings.h"

namespace Engine
{

	/*Implementation of a skybox used by the GraphicsDevice for 3D rendering
	Requires a sigle texture for cubemap
	Image view:
	1: -x
	2: +z
	3: +x
	4: -z
	5: +y
	6: -y
	 x,5,xx
	 1,2,3,4
	 x,6,xx
	  */
	struct Skybox
	{
		std::string path;
	};

}