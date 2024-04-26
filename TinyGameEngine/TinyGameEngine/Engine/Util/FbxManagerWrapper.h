#pragma once
#include "fbxsdk.h"
#include <vector>
#include <memory>
#include "Vertex.h"

class FbxManagerWrapper
{
public:
	FbxManagerWrapper();
	FbxManagerWrapper(const FbxManagerWrapper&) = delete;
	FbxManagerWrapper& operator=(const FbxManagerWrapper&) = delete;
	~FbxManagerWrapper();
	std::shared_ptr<FbxScene> ReadFromFilepath(const char* filepath);
	void LoadMeshesByFilename(const char* filepath, std::shared_ptr<class MeshNode> rootNode);
	void ProcessFbxTree(FbxNode* fbxnode, std::shared_ptr<MeshNode> node);
	void ConstructMesh(FbxNode* fbxMesh, std::shared_ptr<MeshNode> node);
	void GetMaterialTexture(const FbxProperty& prop, std::shared_ptr<MeshNode> node, int& uniqueTextureIndex);
private:
	FbxManager* lSdkManager;
	FbxImporter* lImporter;
};