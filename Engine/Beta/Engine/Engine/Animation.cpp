#include "Animation.h"


using namespace Engine;

Engine::Animation::Animation()
{
}

Engine::Animation::Animation(std::vector<KeyFrame> keyframes)
{
	m_keyframes = keyframes;
}

KeyFrame Engine::Animation::GetKeyFrame(int index)
{
	return m_keyframes[index];
}
