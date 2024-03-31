#pragma once
#include <memory>

class ModelImporter
{
public:
	static void ImportFromFilepath(const char* filepath, std::shared_ptr<class TreeNode> rootNode);
};