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
	void LoadModelWithFilepath(const char* filepath, std::shared_ptr<class TreeNode> rootNode);
	void ProcessFbxTree(FbxNode* fbxnode, std::shared_ptr<TreeNode> node);
	void LoadMeshData(FbxNode* rootNode,
		std::vector<Vertex>& vertices, 
		std::vector<unsigned int>& indices,
		std::vector<DirectX::XMFLOAT2>& uvs);
	void ConstructMesh(FbxNode* fbxMesh, std::shared_ptr<TreeNode> node);

private:
	FbxManager* lSdkManager;
	FbxImporter* lImporter;
};