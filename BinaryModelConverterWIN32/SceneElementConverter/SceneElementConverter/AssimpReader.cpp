#include "AssimpReader.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

using namespace std;
using namespace SEF;

bool AssimpReader::ImportAnimation(const std::string & pFile, Animation& animation)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		//DoTheErrorLogging(importer.GetErrorString());
		return false;
	}
	auto m_GlobalInverseTransform = scene->mRootNode->mTransformation;
	m_GlobalInverseTransform.Inverse();
	int animCount = scene->mNumAnimations;

	for (int animIndex = 0; animIndex < animCount; animIndex++)
	{
		auto* assimpAnim = scene->mAnimations[animIndex];
		std::string name = assimpAnim->mName.C_Str();
		int channelCount = assimpAnim->mNumChannels;
		for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
		{
			auto* channel = assimpAnim->mChannels[channelIndex];
			int keyCount = channel->mNumPositionKeys;
			animation.SetTimestampCount(keyCount);
			vector<AnimationKey> keyCluster;
			std::string name = channel->mNodeName.C_Str();
			keyCluster.resize(keyCount);
			for (int posIndex = 0; posIndex < keyCount; posIndex++)
			{
				AnimationKey& key = keyCluster[posIndex];
				key.Timestamp = (float)channel->mPositionKeys[posIndex].mTime;

				auto position = channel->mPositionKeys[posIndex].mValue;
				key.Position.x = position.x;
				key.Position.y = position.y;
				key.Position.z = position.z;

				auto rotation = channel->mRotationKeys[posIndex].mValue;
				key.Quartenion.x = rotation.x;
				key.Quartenion.y = rotation.y;
				key.Quartenion.z = rotation.z;
				key.Quartenion.w = rotation.w;
			}
			animation.AddCluster(name, keyCluster);
		}
	}
	// Now we can access the file's contents. 
	//DoTheSceneProcessing(scene);
	// We're done. Everything will be cleaned up by the importer destructor
	return scene->HasAnimations();
}
