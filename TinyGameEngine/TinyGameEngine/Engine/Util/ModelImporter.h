#pragma once
#include "FbxManagerWrapper.h"
#include "AssimpWrapper.h"

#include <memory>

namespace ModelImporter
{
	void ImportFromFilepath(const char* filepath, std::shared_ptr<MeshNode> rootNode);
};