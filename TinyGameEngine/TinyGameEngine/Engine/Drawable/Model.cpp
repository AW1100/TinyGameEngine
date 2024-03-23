#include "Model.h"
#include "../Util/FbxManagerWrapper.h"

TreeNode::TreeNode()
{
	numVertices = 0;
	numIndices = 0;
}

TreeNode::TreeNode(int numOfVertices, int numOfIndices)
{
	numVertices = numOfVertices;
	numIndices = numOfIndices;
}

TreeNode::~TreeNode()
{
}

std::vector<std::shared_ptr<TreeNode>>& TreeNode::GetChildNodes()
{
	return childNodes;
}

Model::Model(const char* filepath)
{
	rootNode = std::make_shared<TreeNode>();
	FbxManagerWrapper fbxm;
	fbxm.LoadModelWithFilepath(filepath, rootNode);

}

Model::~Model()
{

}

void Model::FindRenderables(std::vector<std::shared_ptr<class Drawable>>& meshes, Graphics& gfx)
{
	Traverse(rootNode, meshes, gfx);
}

void Model::Traverse(std::shared_ptr<TreeNode> node, std::vector<std::shared_ptr<Drawable>>& meshes, Graphics& gfx)
{
	if (node->Renderable())
	{
		meshes.push_back(std::make_shared<Mesh>(gfx, node));
	}
	if (node->GetChildNodes().size() == 0)
	{
		return;
	}
	else
	{
		for (int i = 0; i < node->GetChildNodes().size(); i++)
		{
			Traverse(node->GetChildNodes()[i], meshes, gfx);
		}
	}
	return;
}


