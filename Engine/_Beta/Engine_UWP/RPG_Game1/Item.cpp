#include "pch.h"
#include "Item.h"
#include <Lime_Engine\EngineResources.h>

using namespace Graphics;
using namespace LIME_ENGINE;

Item::Item()
{
}

Item::~Item()
{
}

void Item::Animate()
{
	static int begin = 9;
	static int val = 0;

	double deltatime = EngineResources::GetTimer()->GetElapsedSeconds();
	static int frame = 0;
	static double totalTime = deltatime;
	static double totalTime2 = deltatime;
	if (totalTime > ANIM_DURATION * 0.5f)
	{
		frame++;
		frame = frame % 8;
		totalTime = 0.0;

		val += 1;
		val = val % 5;
		SetImageAtlasPosition(begin + val, 26);
	}
	float height = 10.0f * (float)cos(2.0 * totalTime2);
	SetHeightOffset(height);
	totalTime += deltatime;
	totalTime2 += deltatime;
}
