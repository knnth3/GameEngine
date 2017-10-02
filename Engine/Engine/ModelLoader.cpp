#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <Windows.h>

std::shared_ptr<Lime::ModelData> Lime::ModelLoader::LoadModel(std::string filename, unsigned int id)
{
	return nullptr;
}

unsigned int Lime::ModelLoader::AssignID()
{
	static int ID = 0;
	return ID++;
}
