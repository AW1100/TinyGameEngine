#include "Model.h"
#include "../Util/ModelImporter.h"

TreeNode::TreeNode()
{
}

TreeNode::~TreeNode()
{
}

std::vector<std::shared_ptr<TreeNode>>& TreeNode::GetChildNodes()
{
	return childNodes;
}

Model::Model(const char* filepath, DirectX::XMFLOAT3 trans)
{
	rootNode = std::make_shared<TreeNode>();
	ModelImporter::ImportFromFilepath(filepath, rootNode);
	translation = trans;
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
		meshes.push_back(std::make_shared<Mesh>(gfx, node, translation));
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

void Model::SetTranslation(const DirectX::XMFLOAT3& pos)
{
	translation = pos;
}


