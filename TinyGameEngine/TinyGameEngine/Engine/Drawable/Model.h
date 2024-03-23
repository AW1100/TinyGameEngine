#pragma once
#include "Mesh.h"
#include "../Util/Vertex.h"

#include <vector>
#include <memory>


class TreeNode
{
public:
	TreeNode();
	TreeNode(int numOfVertices, int numOfIndices);
	~TreeNode();
	std::vector<std::shared_ptr<TreeNode>>& GetChildNodes();
	inline bool Renderable() const { return renderable; }
	inline void SetRenderable() { renderable = true; }

private:
	std::vector<std::shared_ptr<TreeNode>> childNodes;
	bool renderable = false;
	int numVertices;
	int numIndices;

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<const wchar_t*> textureFilenames;
};

class Model
{
public:
	Model(const char* filepath);
	~Model();
	void FindRenderables(std::vector<std::shared_ptr<class Drawable>>& meshes, Graphics& gfx);
	void Traverse(std::shared_ptr<TreeNode> rootNode, std::vector<std::shared_ptr<Drawable>>& meshes, Graphics& gfx);


private:
	std::shared_ptr<TreeNode> rootNode = nullptr;

};