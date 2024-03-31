#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

class AssimpWrapper
{
public:
	static void LoadMeshesByFilename(const char* filepath, std::shared_ptr<class TreeNode> rootNode);
	static void ProcessNode(aiNode* objNode, const aiScene* scene, std::shared_ptr<class TreeNode> node);
	static void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<class TreeNode> node);
};