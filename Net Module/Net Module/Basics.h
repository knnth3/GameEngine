#pragma once
#include "EPacket.h"
#include <vector>
#include <memory>
#include <queue>

typedef std::shared_ptr<std::queue<EPacket>> TQueue;

enum queueType
{
	inqueue = 0,
	outqueue = 1
};