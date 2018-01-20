#pragma once
#include "Animation.h"

namespace Engine
{
	class Animator
	{
	public:
		Animator();
		void LoadAnimation(const Animation& anim);
		void Update(const float elapsed);

	private:
		Animation m_animation;
	};
}
