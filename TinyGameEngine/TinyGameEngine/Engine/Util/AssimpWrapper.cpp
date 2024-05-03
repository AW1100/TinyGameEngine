#include "AssimpWrapper.h"
#include "Log.h"
#include <string>

void AssimpWrapper::LoadMeshesByFilename(const char* filepath, std::shared_ptr<MeshNode> rootNode)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, 
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		//aiProcess_ConvertToLeftHanded |
		aiProcess_CalcTangentSpace
	);

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


	/*for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		auto index0 = face.mIndices[0];
		auto index1 = face.mIndices[1];
		auto index2 = face.mIndices[2];
		auto p0 = mesh->mVertices[index0];
		auto t0 = mesh->mTextureCoords[0][index0];
		auto n0 = mesh->mNormals[index0];
		auto p1 = mesh->mVertices[index1];
		auto t1 = mesh->mTextureCoords[0][index1];
		auto n1 = mesh->mNormals[index1];
		auto p2 = mesh->mVertices[index2];
		auto t2 = mesh->mTextureCoords[0][index2];
		auto n2 = mesh->mNormals[index2];


		auto e0 = p1 - p0;
		auto e1 = p2 - p0;
		auto d0 = t1 - t0;
		auto d1 = t2 - t0;
		float invDet = 1.0f / (d0.x * d1.y - d1.x * d0.y);
		auto tangent = invDet * (d1.y * e0 - d0.y * e1);
		auto bitangent = invDet * (-d1.x * e0 + d0.x * e1);
		
		node->vertices->Store(p0);
		node->vertices->Store(t0);
		node->vertices->Store(n0);
		if (bUseNormalMap)
		{
			node->vertices->Store(tangent);
			node->vertices->Store(bitangent);
		}
		node->vertices->Store(p1);
		node->vertices->Store(t1);
		node->vertices->Store(n1);
		if (bUseNormalMap)
		{
			node->vertices->Store(tangent);
			node->vertices->Store(bitangent);
		}
		node->vertices->Store(p2);
		node->vertices->Store(t2);
		node->vertices->Store(n2);
		if (bUseNormalMap)
		{
			node->vertices->Store(tangent);
			node->vertices->Store(bitangent);
		}

		node->indices.push_back(index0);
		node->indices.push_back(index1);
		node->indices.push_back(index2);
	}*/
	
}
