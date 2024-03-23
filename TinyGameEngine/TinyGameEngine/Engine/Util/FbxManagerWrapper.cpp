#include "FbxManagerWrapper.h"
#include "../Drawable/Model.h"
#include "DirectXTK\WICTextureLoader.h"

#include <cwchar>
#include <iterator>

FbxManagerWrapper::FbxManagerWrapper()
{
    lSdkManager = FbxManager::Create();

    // Create the IO settings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    lImporter = FbxImporter::Create(lSdkManager, "");
}

FbxManagerWrapper::~FbxManagerWrapper()
{
    if (lImporter != nullptr) {
        lImporter->Destroy();
        lImporter = nullptr;
    }
    if (lSdkManager != nullptr) {
        lSdkManager->Destroy();
        lSdkManager = nullptr;
    }
}

std::shared_ptr<FbxScene> FbxManagerWrapper::ReadFromFilepath(const char* filepath)
{
    if (!lImporter->Initialize(filepath, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    std::shared_ptr<FbxScene> lScene(FbxScene::Create(lSdkManager, "myScene"), [](FbxScene* ptr) {if (ptr != nullptr) {ptr->Destroy();}});

    // Import the contents of the file into the scene.
    lImporter->Import(lScene.get());

    return lScene;
}

void FbxManagerWrapper::LoadModelWithFilepath(const char* filepath, std::shared_ptr<TreeNode> rootNode)
{
    if (!lImporter->Initialize(filepath, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    std::shared_ptr<FbxScene> lScene(FbxScene::Create(lSdkManager, "myScene"), [](FbxScene* ptr) {if (ptr != nullptr) { ptr->Destroy(); }});
    //FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
    lImporter->Import(lScene.get());
    ProcessFbxTree(lScene->GetRootNode(), rootNode);
}

void FbxManagerWrapper::ProcessFbxTree(FbxNode* fbxnode, std::shared_ptr<TreeNode> node)
{
    int numOfChildren = fbxnode->GetChildCount();
    if (numOfChildren == 0)
    {
        if (fbxnode->GetNodeAttribute() && fbxnode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            FbxMesh* mesh = (FbxMesh*)fbxnode->GetNodeAttribute();
            if (mesh)
            {

                int numVertices = mesh->GetControlPointsCount();
                int numPolygons = mesh->GetPolygonCount();
                std::shared_ptr<TreeNode> child = std::make_shared<TreeNode>(numVertices, numPolygons);
                child->SetRenderable();
                ConstructMesh(fbxnode, child);
                node->GetChildNodes().push_back(child);
            }
        }
        return;
    }
    else
    {
        for (int i = 0; i < fbxnode->GetChildCount(); i++) {
            auto child = fbxnode->GetChild(i);           
            ProcessFbxTree(child, node);           
        }       
    }
    return;
}


void FbxManagerWrapper::LoadMeshData(FbxNode* rootNode, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<DirectX::XMFLOAT2>& uvs)
{
    auto extractVertices = [&vertices](FbxMesh* m) -> void {
        int numVertices = m->GetControlPointsCount();
        for (int i = 0; i < numVertices; i++) {
            FbxVector4 vertex = m->GetControlPointAt(i);
            // Do something with vertex, e.g., store it in your custom data structure
            Vertex v;
            v.pos.x = float(vertex[0]);
            v.pos.y = float(vertex[1]);
            v.pos.z = float(vertex[2]);
            vertices.push_back(v);
        }
        };

    auto extractIndices = [&indices](FbxMesh* m)->void {
        int numPolygons = m->GetPolygonCount();
        for (int i = 0; i < numPolygons; i++) {
            int polygonSize = m->GetPolygonSize(i);
            for (int j = 0; j < polygonSize; j++) {
                int index = m->GetPolygonVertex(i, j);
                // Do something with index, e.g., store it in your custom data structure
                indices.push_back(index);
            }
        }
        };

    auto extractTextureCoord = [&](FbxMesh* m)->void {
        FbxStringList uvSetNameList;
        m->GetUVSetNames(uvSetNameList);
        for (int i = 0; i < uvSetNameList.GetCount(); i++) {
            const char* uvSetName = uvSetNameList.GetStringAt(i);
            const FbxGeometryElementUV* uvElement = m->GetElementUV(uvSetName);
            if (!uvElement)
                continue;

            for (int j = 0; j < m->GetPolygonCount(); j++) {
                for (int k = 0; k < m->GetPolygonSize(j); k++) {
                    FbxVector2 uv;
                    bool unmappedUV;
                    m->GetPolygonVertexUV(j, k, uvSetName, uv, unmappedUV);
                    DirectX::XMFLOAT2 tex;
                    tex.x = uv[0];
                    tex.y = uv[1];
                    uvs.push_back(tex);
                }
            }
        }
        };

    auto processNode = [&](FbxNode* node) -> void {
        if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
            FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();

            extractVertices(mesh);
            extractIndices(mesh);
            extractTextureCoord(mesh);

            FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
            if (normalElement) {
                for (int i = 0; i < mesh->GetPolygonCount(); i++) {
                    for (int j = 0; j < mesh->GetPolygonSize(i); j++) {
                        FbxVector4 normal;
                        if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
                            int normalIndex = i * mesh->GetPolygonSize(i) + j;
                            if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                                normal = normalElement->GetDirectArray().GetAt(normalIndex);
                            }
                            else if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                                int id = normalElement->GetIndexArray().GetAt(normalIndex);
                                normal = normalElement->GetDirectArray().GetAt(id);
                            }
                            // Do something with normal
                        }
                    }
                }
            }

            // Handle Embedded Textures (simplified)
            int materialCount = node->GetMaterialCount();
            for (int i = 0; i < materialCount; i++) {
                FbxSurfaceMaterial* material = node->GetMaterial(i);
                // This is a simplified example; in a real scenario, you would check the type of the material
                // and extract textures for different material properties (diffuse, specular, etc.)
                FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
                int textureCount = prop.GetSrcObjectCount<FbxTexture>();
                for (int j = 0; j < textureCount; j++) {
                    FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(j));
                    if (texture) {
                        // Do something with the texture, like extracting file path or texture name
                        const char* textureName = texture->GetName();
                        // Note: For embedded textures, you might need to access the file data directly
                        // rather than just the name or path
                        int aa = 0;
                    }
                }
            }
        }
        };

    // Recursively process each node in the scene
    auto processRecursive = [&](FbxNode* node, auto& selfRef) -> void {
        processNode(node);
        auto nn = node->GetChildCount();
        for (int i = 0; i < node->GetChildCount(); i++) {
            selfRef(node->GetChild(i), selfRef); // Recursive call
        }
        };

    processRecursive(rootNode, processRecursive);
}

void FbxManagerWrapper::ConstructMesh(FbxNode* fbxNode, std::shared_ptr<TreeNode> node)
{
    FbxMesh* fbxMesh = (FbxMesh*)fbxNode->GetNodeAttribute();

    std::vector<DirectX::XMFLOAT3> positions;
    int numVertices = fbxMesh->GetControlPointsCount();
    for (int i = 0; i < numVertices; i++) {
        FbxVector4 vertex = fbxMesh->GetControlPointAt(i);
        // Do something with vertex, e.g., store it in your custom data structure
        positions.push_back(DirectX::XMFLOAT3(vertex[0], vertex[1], vertex[2]));
    }

    FbxStringList uvSetNameList;
    fbxMesh->GetUVSetNames(uvSetNameList);
    std::vector<DirectX::XMFLOAT2> texCoords;
    for (int i = 0; i < uvSetNameList.GetCount(); i++) {
        const char* uvSetName = uvSetNameList.GetStringAt(i);
        const FbxGeometryElementUV* uvElement = fbxMesh->GetElementUV(uvSetName);
        if (!uvElement)
            continue;

        for (int j = 0; j < fbxMesh->GetPolygonCount(); j++) {
            for (int k = 0; k < fbxMesh->GetPolygonSize(j); k++) {
                FbxVector2 uv;
                bool unmappedUV;
                fbxMesh->GetPolygonVertexUV(j, k, uvSetName, uv, unmappedUV);
                DirectX::XMFLOAT2 tex;
                tex.x = uv[0];
                tex.y = uv[1];
                texCoords.push_back(tex);
            }
        }
    }

    std::vector<DirectX::XMFLOAT3> normals;
    FbxGeometryElementNormal* normalElement = fbxMesh->GetElementNormal();
    if (normalElement) {
        for (int i = 0; i < fbxMesh->GetPolygonCount(); i++) {
            for (int j = 0; j < fbxMesh->GetPolygonSize(i); j++) {
                FbxVector4 normal;
                if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
                    int normalIndex = i * fbxMesh->GetPolygonSize(i) + j;
                    if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                        normal = normalElement->GetDirectArray().GetAt(normalIndex);
                    }
                    else if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                        int id = normalElement->GetIndexArray().GetAt(normalIndex);
                        normal = normalElement->GetDirectArray().GetAt(id);
                    }
                    
                }
                normals.push_back(DirectX::XMFLOAT3(normal[0], normal[1], normal[2]));
            }
        }
    }

    int numPolygons = fbxMesh->GetPolygonCount();
    int uniqueTextureIndex = 0;
    for (int i = 0; i < numPolygons; i++) {
        int materialIndex = fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(i);
        int materialCount = fbxMesh->GetNode()->GetMaterialCount();
        //node->textureFilenames.resize(materialCount);
        FbxSurfaceMaterial* material = fbxMesh->GetNode()->GetMaterial(materialIndex);
        if (material) {
            // Assume we are looking for the diffuse texture
            FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

            int numTextures = prop.GetSrcObjectCount<FbxTexture>();
            for (int j = 0; j < numTextures; j++) {
                FbxTexture* texture = prop.GetSrcObject<FbxTexture>(j);
                if (texture) {
                    FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
                    if (fileTexture) {
                        // If the cast is successful, retrieve the relative filename
                        const char* relativeFilename = fileTexture->GetFileName();
                        int bufferLen = MultiByteToWideChar(CP_UTF8, 0, relativeFilename, -1, nullptr, 0);
                        wchar_t* wideString = new wchar_t[bufferLen];
                        MultiByteToWideChar(CP_UTF8, 0, relativeFilename, -1, wideString, bufferLen);

                        auto it = std::find_if(node->textureFilenames.begin(), node->textureFilenames.end(), [wideString](const wchar_t* str) {
                            return wcscmp(str, wideString) == 0;
                            });
                        if (it == node->textureFilenames.end())
                        {
                            node->textureFilenames.push_back(wideString);
                            uniqueTextureIndex = node->textureFilenames.size();
                        }
                        else
                        {
                            uniqueTextureIndex = std::distance(node->textureFilenames.begin(), it);
                            
                        }
                    }
                }
            }
        }

        int polygonSize = fbxMesh->GetPolygonSize(i);
        if (polygonSize != 3)
        {
            // TODO - dealing with other shapes
            return;
        }
        auto firstIndex = fbxMesh->GetPolygonVertex(i, 0);
        auto secondIndex = fbxMesh->GetPolygonVertex(i, 1);
        auto thirdIndex = fbxMesh->GetPolygonVertex(i, 2);
        Vertex v0, v1, v2;
        v0.pos = positions[firstIndex];
        v0.texCoord = DirectX::XMFLOAT3(texCoords[i * 3].x, (1.0f-texCoords[i * 3].y), uniqueTextureIndex);
        v0.normal = normals[i*3];
        v1.pos = positions[secondIndex];
        v1.texCoord = DirectX::XMFLOAT3(texCoords[i * 3 + 1].x, (1.0f - texCoords[i * 3+1].y), uniqueTextureIndex);
        v1.normal = normals[i*3+1];
        v2.pos = positions[thirdIndex];
        v2.texCoord = DirectX::XMFLOAT3(texCoords[i * 3 + 2].x, (1.0f - texCoords[i * 3+2].y), uniqueTextureIndex);
        v2.normal = normals[i*3+2];
        node->vertices.push_back(v0);
        node->vertices.push_back(v1);
        node->vertices.push_back(v2);

        node->indices.push_back(i * 3);
        node->indices.push_back(i * 3 + 1);
        node->indices.push_back(i * 3 + 2);
    }

}


