#include "FBXReader.h"
#include "include\SEStream.h"
#include <iostream>


#define ORIGINAL_FILE "C:/Users/Kenneth/Music/Cube.obj"
#define NEW_FILE "C:/Users/Kenneth/Music/Cube.sef"

using namespace std;

bool TestFBXReadProgram(vector<MeshData>& meshArr, vector<Skeleton>& skeletonArr);
bool TestSEFReadProgram(MeshData & mesh, Skeleton & skeleton);
bool TestSEFWriteProgram(const MeshData* mesh, const Skeleton* skeleton);

int main()
{
	string ch;
	vector<MeshData> meshArr;
	vector<Skeleton> skeletonArr;
	if (TestFBXReadProgram(meshArr, skeletonArr))
	{
		MeshData* m = (meshArr.empty()) ? nullptr : &meshArr[0];
		Skeleton* s = (skeletonArr.empty()) ? nullptr : &skeletonArr[0];
		TestSEFWriteProgram(m,s);
	}

	//Read .sef
	MeshData readMesh;
	Skeleton readSkeleton;
	TestSEFReadProgram(readMesh, readSkeleton);

	printf("Press enter key to exit...\n");
	getline(cin, ch);
	return 0;
}

bool TestFBXReadProgram(vector<MeshData>& meshArr, vector<Skeleton>& skeletonArr)
{
	FBXReader IO;
	string error;
	if (!IO.ReadFile(ORIGINAL_FILE, meshArr, skeletonArr, error))
	{
		cout << error << endl;
		return false;
	}

	cout << "File read success from FBXReader!" << endl;
	return true;
}

bool TestSEFReadProgram(MeshData& mesh, Skeleton& skeleton)
{
	SEStream fileIO;
	bool result = false;
	try
	{
		fileIO.ReadFile(NEW_FILE, &mesh, &skeleton);
		cout << "File read success from SEStream!" << endl;
		result = true;
	}
	catch (const std::runtime_error e)
	{
		cout << e.what() << endl;
	}
	return result;
}

bool TestSEFWriteProgram(const MeshData* mesh, const Skeleton* skeleton)
{
	SEStream fileIO;
	bool result = false;
	try
	{
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
