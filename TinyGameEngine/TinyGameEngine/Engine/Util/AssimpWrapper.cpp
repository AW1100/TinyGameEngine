#include "AssimpWrapper.h"
#include "Log.h"
#include <string>

void AssimpWrapper::LoadMeshesByFilename(const char* filepath, std::shared_ptr<MeshNode> rootNode)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, 
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		Log::GetInstance().AddLog(std::string("Failed to load " + std::string(filepath)).c_str());
	}

	ProcessNode(scene->mRootNode, scene, rootNode);
}

void AssimpWrapper::ProcessNode(aiNode* objNode, const aiScene* scene, std::shared_ptr<MeshNode> node)
{
	for (unsigned int i = 0; i < objNode->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[objNode->mMeshes[i]];
		auto meshName = mesh->mName.C_Str();
		auto childNode = std::make_shared<MeshNode>(meshName);
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
	auto ProcessMaterials = [&](aiTextureType type, std::vector<const wchar_t*>& filenames, bool setIndex) -> void {
		auto matCount = material->GetTextureCount(type);
		aiString str1;
		material->GetTexture(type, 0, &str1);
		for (unsigned int i = 0; i < matCount; i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			int bufferLen = MultiByteToWideChar(CP_UTF8, 0, str.C_Str(), -1, nullptr, 0);
			wchar_t* wideString = new wchar_t[bufferLen];
			MultiByteToWideChar(CP_UTF8, 0, str.C_Str(), -1, wideString, bufferLen);

			auto it = std::find_if(filenames.begin(), filenames.end(), [wideString](const wchar_t* str) {
				return wcscmp(str, wideString) == 0;
				});
			if (it == filenames.end())
			{
				filenames.push_back(wideString);
				if (setIndex)
					uniqueTextureIndex = filenames.size();
			}
			else
			{
				if (setIndex)
					uniqueTextureIndex = std::distance(filenames.begin(), it);
			}

			//filenames.push_back(wideString);
		}
		};

	if (material->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) > 0)
	{
		node->vertexType = node->vertexType | Diffuse;
		ProcessMaterials(aiTextureType::aiTextureType_DIFFUSE, node->diffuse, true);
	}
	
	if (material->GetTextureCount(aiTextureType::aiTextureType_SPECULAR) > 0)
	{
		node->vertexType = node->vertexType | Specular;
		ProcessMaterials(aiTextureType::aiTextureType_SPECULAR, node->specular, false);
	}
	bool bUseNormalMap = false;
	if (material->GetTextureCount(aiTextureType::aiTextureType_NORMALS) > 0)
	{
		bUseNormalMap = true;
		node->vertexType = node->vertexType | NormalMap;
		ProcessMaterials(aiTextureType::aiTextureType_NORMALS, node->normalMap, false);
	}
	

	if (mesh->mNumVertices > 0u)
	{
		node->vertices->GetLayout().Append({ElementType::Position3D});
	}
	if (mesh->mNumUVComponents[0] > 0u)
	{
		node->vertices->GetLayout().Append({ ElementType::Texture2D });
	}
	if (mesh->HasNormals())
	{
		node->vertices->GetLayout().Append({ ElementType::Normal });
	}
	if (mesh->HasTangentsAndBitangents() && bUseNormalMap)
	{
		node->vertices->GetLayout().Append({ ElementType::Tangent });
		node->vertices->GetLayout().Append({ ElementType::Bitangent });
	}
	else
	{
		bUseNormalMap = false;
	}
	

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
	{
		aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);

		auto i0 = face.mIndices[0];
		auto i1 = face.mIndices[1];
		auto i2 = face.mIndices[2];
		Vertex v0, v1, v2;

		for (int i = 0; i < face.mNumIndices; i++)
		{
			auto index = face.mIndices[i];
			if (mesh->mNumVertices > 0u)
			{
				node->vertices->Store(mesh->mVertices[index]);
			}
			if (mesh->mNumUVComponents[0] > 0u)
			{
				node->vertices->Store(mesh->mTextureCoords[0][index].x);
				node->vertices->Store(mesh->mTextureCoords[0][index].y);
				node->vertices->Store(uniqueTextureIndex);
			}
			if (mesh->HasNormals())
			{
				node->vertices->Store(mesh->mNormals[index]);
			}	
			if (bUseNormalMap)
			{
				node->vertices->Store(mesh->mTangents[index]);
				node->vertices->Store(mesh->mBitangents[index]);
			}
			node->indices.push_back(index);
		}
	}	
}
