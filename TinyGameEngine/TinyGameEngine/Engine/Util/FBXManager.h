#pragma once
#include "fbxsdk.h"
#include <vector>
#include <memory>
#include "Vertex.h"

class FBXManager
{
public:
	FBXManager();
	FBXManager(const FBXManager&) = delete;
	FBXManager& operator=(const FBXManager&) = delete;
	~FBXManager();
	std::shared_ptr<FbxScene> ReadFromFilepath(const char* filepath);
	void LoadMeshData(FbxNode* rootNode,
		std::vector<Vertex>& vertices, 
		std::vector<unsigned int>& indices,
		std::vector<UV>& uvs);

private:
	FbxManager* lSdkManager;
	FbxImporter* lImporter;
};