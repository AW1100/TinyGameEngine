#include "ModelImporter.h"

#include <cstring>

void ModelImporter::ImportFromFilepath(const char* filepath, std::shared_ptr<MeshNode> rootNode)
{
	const char* ext = strrchr(filepath, '.');
	if (strcmp(ext, ".fbx") == 0)
	{
		FbxManagerWrapper fbxm;
		fbxm.LoadMeshesByFilename(filepath, rootNode);
	}
	else if (strcmp(ext, ".obj") == 0)
	{
		AssimpWrapper::LoadMeshesByFilename(filepath, rootNode);
	}
}
