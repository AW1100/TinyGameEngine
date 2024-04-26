#include "AssimpWrapper.h"
#include "Log.h"
#include <string>

void AssimpWrapper::LoadMeshesByFilename(const char* filepath, std::shared_ptr<MeshNode> rootNode)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate |
		aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		Log::GetInstance().AddLog(std::string("Failed to load " + std::string(filepath)).c_str());
	}

	ProcessNode(scene->mRootNode, scene, rootNode);
}

void AssimpWrapper::ProcessNode(aiNode* objNode, const aiScene* scene, std::shared_ptr<MeshNode> node)
{
	for (unsigned int i = 0; i < objNode->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[objNode->mMeshes[i]];
		auto childNode = std::make_shared<MeshNode>();
		node->GetChildNodes().push_back(childNode);
		ProcessMesh(mesh, scene, childNode);
	}

	for (unsigned int j = 0; j < objNode->mNumChildren; j++) {
		auto childNode = std::make_shared<MeshNode>();
		node->GetChildNodes().push_back(childNode);
		ProcessNode(objNode->mChildren[j], scene, childNode);
	}
}

void AssimpWrapper::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::shared_ptr<MeshNode> node)
{
	node->SetRenderable();
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	
	//assert(matCount == 1);
	unsigned int uniqueTextureIndex = 0;
	auto ProcessMaterials = [&](aiTextureType type, std::vector<const wchar_t*> filenames) -> void {
		auto matCount = material->GetTextureCount(type);
		
		for (unsigned int i = 0; i < matCount; i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			int bufferLen = MultiByteToWideChar(CP_UTF8, 0, str.C_Str(), -1, nullptr, 0);
			wchar_t* wideString = new wchar_t[bufferLen];
			MultiByteToWideChar(CP_UTF8, 0, str.C_Str(), -1, wideString, bufferLen);

			auto it = std::find_if(node->diffuse.begin(), node->diffuse.end(), [wideString](const wchar_t* str) {
				return wcscmp(str, wideString) == 0;
				});
			if (it == node->diffuse.end())
			{
				node->diffuse.push_back(wideString);
				uniqueTextureIndex = node->diffuse.size();
			}
			else
			{
				uniqueTextureIndex = std::distance(node->diffuse.begin(), it);

			}

			filenames.push_back(wideString);
		}
		};

	ProcessMaterials(aiTextureType::aiTextureType_DIFFUSE, node->diffuse);
	//ProcessMaterials(aiTextureType::aiTextureType_SPECULAR, node->specular);
	

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
	{
		aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);

		auto i0 = face.mIndices[0];
		auto i1 = face.mIndices[1];
		auto i2 = face.mIndices[2];
		Vertex v0, v1, v2;
		if (mesh->mNumVertices > 0)
		{
			v0.pos.x = mesh->mVertices[i0].x;
			v0.pos.y = mesh->mVertices[i0].y;
			v0.pos.z = mesh->mVertices[i0].z;
			v1.pos.x = mesh->mVertices[i1].x;
			v1.pos.y = mesh->mVertices[i1].y;
			v1.pos.z = mesh->mVertices[i1].z;
			v2.pos.x = mesh->mVertices[i2].x;
			v2.pos.y = mesh->mVertices[i2].y;
			v2.pos.z = mesh->mVertices[i2].z;

		}

		if (mesh->mNumUVComponents[0] > 0u)
		{
			v0.texCoord.x = mesh->mTextureCoords[0][i0].x;
			v0.texCoord.y = mesh->mTextureCoords[0][i0].y;
			v0.texCoord.z = uniqueTextureIndex;
			v1.texCoord.x = mesh->mTextureCoords[0][i1].x;
			v1.texCoord.y = mesh->mTextureCoords[0][i1].y;
			v1.texCoord.z = uniqueTextureIndex;
			v2.texCoord.x = mesh->mTextureCoords[0][i2].x;
			v2.texCoord.y = mesh->mTextureCoords[0][i2].y;
			v2.texCoord.z = uniqueTextureIndex;
		}

		if (mesh->HasNormals())
		{
			v0.normal.x = mesh->mNormals[i0].x;
			v0.normal.y = mesh->mNormals[i0].y;
			v0.normal.z = mesh->mNormals[i0].z;
			v1.normal.x = mesh->mNormals[i1].x;
			v1.normal.y = mesh->mNormals[i1].y;
			v1.normal.z = mesh->mNormals[i1].z;
			v2.normal.x = mesh->mNormals[i2].x;
			v2.normal.y = mesh->mNormals[i2].y;
			v2.normal.z = mesh->mNormals[i2].z;
		}
		node->vertices.push_back(v0);
		node->vertices.push_back(v1);
		node->vertices.push_back(v2);
		node->indices.push_back(i0);
		node->indices.push_back(i1);
		node->indices.push_back(i2);
	}

	
}
