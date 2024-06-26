#include "Model.h"
#include "../Util/ModelImporter.h"

MeshNode::MeshNode()
{
}

MeshNode::MeshNode(const char* meshName)
	:name(meshName)
{
	/*VertexLayout v;
	v.Append({ ElementType::Position3D });
	v.Append({ ElementType::Texture2D });
	v.Append({ ElementType::Normal });*/
	vertices = new DynamicVertex();
}

MeshNode::~MeshNode()
{
}

std::vector<std::shared_ptr<MeshNode>>& MeshNode::GetChildNodes()
{
	return childNodes;
}

Model::Model(const char* filepath, DirectX::XMFLOAT3 trans, MeshType type)
{
	rootNode = std::make_shared<MeshNode>();
	ModelImporter::ImportFromFilepath(filepath, rootNode);
	translation = trans;
	meshType = type;
}

Model::~Model()
{

}

void Model::FindRenderables(std::vector<Drawable*>& meshes, Graphics& gfx)
{
	Traverse(rootNode, meshes, gfx);
}

void Model::Traverse(std::shared_ptr<MeshNode> node, std::vector<Drawable*>& meshes, Graphics& gfx)
{
	if (node->Renderable())
	{
		// TODO memory pool
		if (node->vertexType != 0) 
			meshes.push_back(new Mesh(gfx, node, translation, meshType, node->name, node->vertexType, useOutline));
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


