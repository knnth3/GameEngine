#include "FBXReader.h"
#include "include\SEStream.h"
#include <iostream>

using namespace std;

bool TestFBXReadProgram(vector<MeshData>& meshArr, vector<Skeleton>& skeletonArr);
bool TestSEFReadProgram(MeshData & mesh, Skeleton & skeleton);
bool TestSEFWriteProgram(const MeshData& mesh, const Skeleton& skeleton);

int main()
{
	string ch;
	vector<MeshData> meshArr;
	vector<Skeleton> skeletonArr;
	if(TestFBXReadProgram(meshArr, skeletonArr))
		TestSEFWriteProgram(meshArr[0], skeletonArr[0]);

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
	if (!IO.ReadFile("C:/Users/Kenneth/Music/model.fbx", meshArr, skeletonArr, error))
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
		fileIO.ReadFile("C:/Users/Kenneth/Music/model.sef", &mesh, &skeleton);
		cout << "File read success from SEStream!" << endl;
		result = true;
	}
	catch (const std::runtime_error e)
	{
		cout << e.what() << endl;
	}
	return result;
}

bool TestSEFWriteProgram(const MeshData & mesh, const Skeleton & skeleton)
{
	SEStream fileIO;
	bool result = false;
	try
	{
		fileIO.WriteFile("C:/Users/Kenneth/Music/model.sef", &mesh, &skeleton);
		cout << "File write success from SEStream!" << endl;
		result = true;
	}
	catch (const std::runtime_error e)
	{
		cout << e.what() << endl;
	}
	return result;
}
