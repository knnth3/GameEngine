#include "FBXReader.h"
#include "include\SEStream.h"
#include <iostream>
#include "AssimpReader.h"


#define ORIGINAL_FILE "C:/Users/Kenneth/Music/head1.fbx"
#define ANIMATION_FILE "C:/Users/Kenneth/Music/walk_anim.dae"
#define NEW_FILE "C:/Users/Kenneth/Music/head1.sef"

using namespace std;
using namespace SEF;

bool TestFBXReadProgram(vector<MeshData>& meshArr, vector<Skeleton>& skeletonArr, bool attatchment, int skelIndexOffset);
bool TestSEFReadProgram(MeshData & mesh, Skeleton & skeleton, Animation* animation);
bool TestSEFWriteProgram(const MeshData* mesh, const Skeleton* skeleton, const Animation* animation = nullptr);

int main()
{
	string ch;

	//AssimpReader io;
	//Animation writeAnimation;
	//cout << "Read Animation File: "<< io.ImportAnimation(ANIMATION_FILE, writeAnimation) << endl;

	vector<MeshData> meshArr;
	vector<Skeleton> skeletonArr;
	if (TestFBXReadProgram(meshArr, skeletonArr, true, 29))
	{
		MeshData* writeMesh = (meshArr.empty()) ? nullptr : &meshArr[0];
		Skeleton* writeSkeleton = (skeletonArr.empty()) ? nullptr : &skeletonArr[0];
		TestSEFWriteProgram(writeMesh,nullptr, nullptr);
	}

	//Read .sef
	MeshData readMesh;
	Skeleton readSkeleton;
	Animation readAnimation;
	TestSEFReadProgram(readMesh, readSkeleton, &readAnimation);

	printf("Press enter key to exit...\n");
	getline(cin, ch);
	return 0;
}

bool TestFBXReadProgram(vector<MeshData>& meshArr, vector<Skeleton>& skeletonArr, bool attatchment, int skelIndexOffset)
{
	FBXReader IO;
	string error;
	if (!IO.ReadFile(ORIGINAL_FILE, meshArr, skeletonArr, error, attatchment, skelIndexOffset))
	{
		cout << error << endl;
		return false;
	}

	cout << "File read success from FBXReader!" << endl;
	return true;
}

bool TestSEFReadProgram(MeshData& mesh, Skeleton& skeleton, Animation* animation)
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

bool TestSEFWriteProgram(const MeshData* mesh, const Skeleton* skeleton, const Animation* animation)
{
	SEStream fileIO;
	bool result = false;
	try
	{
		if (animation)
			fileIO.AddAnimation(*animation);

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
