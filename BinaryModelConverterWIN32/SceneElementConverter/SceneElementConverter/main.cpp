#include "FBXReader.h"
#include "include\SEStream.h"
#include <iostream>
#include "AssimpReader.h"


#define ORIGINAL_FILE "C:/Users/Kenneth/3D Objects/body_plate_silver_epic.fbx"
#define ANIMATION_FILE1 "C:/Users/Kenneth/3D Objects/ss_biped@unarmed_idle_01.dae"
#define ANIMATION_FILE2 "C:/Users/Kenneth/3D Objects/walk_anim.dae"
#define ANIMATION_FILE3 "C:/Users/Kenneth/3D Objects/ss_biped@unarmed_jump.dae"
#define NEW_FILE "C:/Users/Kenneth/3D Objects/body_plate_silver_epic.sef"
#define IS_ADDON false
#define ADDON_SKELETON_BONE_INDEX 29

using namespace std;
using namespace SEF;

bool TestFBXReadProgram(vector<MeshData>& meshArr, vector<Skeleton>& skeletonArr, bool attatchment, int skelIndex);
bool TestSEFReadProgram(MeshData & mesh, Skeleton & skeleton, std::vector<Animation>* animation);
bool TestSEFWriteProgram(const MeshData* mesh, const Skeleton* skeleton, const std::vector<Animation>* animation = nullptr);

int main()
{
	string ch;

	AssimpReader io;
	std::vector<Animation> writeAnimation;
	writeAnimation.emplace_back();
	cout << "Read Animation File: "<< io.ImportAnimation(ANIMATION_FILE1, writeAnimation.back()) << endl;

	writeAnimation.emplace_back();
	cout << "Read Animation File: " << io.ImportAnimation(ANIMATION_FILE2, writeAnimation.back()) << endl;

	writeAnimation.emplace_back();
	cout << "Read Animation File: " << io.ImportAnimation(ANIMATION_FILE3, writeAnimation.back()) << endl;


	vector<MeshData> meshArr;
	vector<Skeleton> skeletonArr;
	if (TestFBXReadProgram(meshArr, skeletonArr, IS_ADDON, ADDON_SKELETON_BONE_INDEX))
	{
		MeshData* writeMesh = (meshArr.empty()) ? nullptr : &meshArr[0];
		Skeleton* writeSkeleton = (skeletonArr.empty()) ? nullptr : &skeletonArr[0];
		TestSEFWriteProgram(writeMesh, writeSkeleton, &writeAnimation);
	}

	//Read .sef
	MeshData readMesh;
	Skeleton readSkeleton;
	std::vector<Animation> readAnimation;
	TestSEFReadProgram(readMesh, readSkeleton, &readAnimation);

	printf("Press enter key to exit...\n");
	getline(cin, ch);
	return 0;
}

bool TestFBXReadProgram(vector<MeshData>& meshArr, vector<Skeleton>& skeletonArr, bool attatchment, int skelIndex)
{
	FBXReader IO;
	string error;
	if (!IO.ReadFile(ORIGINAL_FILE, meshArr, skeletonArr, error, attatchment, skelIndex))
	{
		cout << error << endl;
		return false;
	}

	cout << "File read success from FBXReader!" << endl;
	return true;
}

bool TestSEFReadProgram(MeshData& mesh, Skeleton& skeleton, std::vector<Animation>* animation)
{
	SEStream fileIO;
	bool result = false;
	try
	{
		fileIO.ReadFile(NEW_FILE, &mesh, &skeleton, animation);
		cout << "File read success from SEStream!" << endl;
		result = true;
	}
	catch (const std::runtime_error e)
	{
		cout << e.what() << endl;
	}
	return result;
}

bool TestSEFWriteProgram(const MeshData* mesh, const Skeleton* skeleton, const std::vector<Animation>* animation)
{
	SEStream fileIO;
	bool result = false;
	try
	{
		if (animation)
		{
			for (const auto& anim : *animation)
			{
				fileIO.AddAnimation(anim);
			}
		}

		fileIO.WriteFile(NEW_FILE, mesh, skeleton);
		cout << "File write success from SEStream!" << endl;
		result = true;
	}
	catch (const std::runtime_error e)
	{
		cout << e.what() << endl;
	}
	return result;
}
