#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

#include "../Drawable/Model.h"

namespace AssimpWrapper
{
	void LoadMeshesByFilename(const char* filepath, std::shared_ptr<MeshNode> rootNode);
	void ProcessNode(aiNode* objNode, const aiScene* scene, std::shared_ptr<MeshNode> node);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<MeshNode> node);
};