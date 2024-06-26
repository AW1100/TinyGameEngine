#pragma once
#include "Mesh.h"
#include "../Util/Vertex.h"
#include "../Util/DynamicVertex.hpp"

#include <vector>
#include <memory>


class MeshNode
{
public:
	MeshNode();
	MeshNode(const char* meshName);
	~MeshNode();
	std::vector<std::shared_ptr<MeshNode>>& GetChildNodes();
	inline bool Renderable() const { return renderable; }
	inline void SetRenderable() { renderable = true; }
	

private:
	std::vector<std::shared_ptr<MeshNode>> childNodes;
	bool renderable = false;

public:
	//std::vector<Vertex> vertices;
	DynamicVertex* vertices;
	std::vector<unsigned int> indices;
	std::vector<const wchar_t*> diffuse;
	std::vector<const wchar_t*> specular;
	std::vector<const wchar_t*> normalMap;
	std::string name;
	unsigned int vertexType = None;

};

class Model
{
public:
	Model(const char* filepath, DirectX::XMFLOAT3 trans, MeshType type);
	~Model();
	void FindRenderables(std::vector<Drawable*>& meshes, Graphics& gfx);
	void Traverse(std::shared_ptr<MeshNode> rootNode, std::vector<Drawable*>& meshes, Graphics& gfx);
	void SetTranslation(const DirectX::XMFLOAT3& pos);
	inline void UseOutline(bool use) { useOutline = use; }

private:
	std::shared_ptr<MeshNode> rootNode = nullptr;
	DirectX::XMFLOAT3 translation;
	MeshType meshType;
	bool useOutline = false;
};