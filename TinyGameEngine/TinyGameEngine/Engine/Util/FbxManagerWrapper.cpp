#include "FbxManagerWrapper.h"
#include "../Drawable/Model.h"
#include "DirectXTK\WICTextureLoader.h"

#include <cwchar>
#include <iterator>
#include <codecvt>

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

void FbxManagerWrapper::LoadMeshesByFilename(const char* filepath, std::shared_ptr<MeshNode> rootNode)
{
    if (!lImporter->Initialize(filepath, -1, lSdkManager->GetIOSettings())) {
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
        exit(-1);
    }

    std::shared_ptr<FbxScene> lScene(FbxScene::Create(lSdkManager, "myScene"), [](FbxScene* ptr) {if (ptr != nullptr) { ptr->Destroy(); }});
    lImporter->Import(lScene.get());

    // Triangulate the meshes
    FbxGeometryConverter lGeomConverter(lSdkManager);
    bool replace = true; // Replace original meshes with triangulated ones
    lGeomConverter.Triangulate(lScene.get(), replace);

    ProcessFbxTree(lScene->GetRootNode(), rootNode);
}

void FbxManagerWrapper::ProcessFbxTree(FbxNode* fbxnode, std::shared_ptr<MeshNode> node)
{
    FbxMesh* mesh = fbxnode->GetMesh();
    int numOfChildren = fbxnode->GetChildCount();
    if (mesh)
    {
        const char* meshName = mesh->GetName();
        /*int bufferLen = MultiByteToWideChar(CP_UTF8, 0, meshName, -1, nullptr, 0);
        wchar_t* wideString = new wchar_t[bufferLen];
        MultiByteToWideChar(CP_UTF8, 0, meshName, -1, wideString, bufferLen);*/
        std::shared_ptr<MeshNode> child = std::make_shared<MeshNode>(meshName);
        child->SetRenderable();
        ConstructMesh(fbxnode, child);
        node->GetChildNodes().push_back(child);
        node = child;
    }

    for (int i = 0; i < numOfChildren; i++) {
        auto child = fbxnode->GetChild(i);
        ProcessFbxTree(child, node);
    }
    return;
}

void FbxManagerWrapper::ConstructMesh(FbxNode* fbxNode, std::shared_ptr<MeshNode> node)
{
    FbxMesh* fbxMesh = (FbxMesh*)fbxNode->GetNodeAttribute();

    std::vector<DirectX::XMFLOAT3> positions;
    int numVertices = fbxMesh->GetControlPointsCount();
    if (numVertices > 0)
    {
        node->vertices->GetLayout().Append({ ElementType::Position3D });
    }
    for (int i = 0; i < numVertices; i++) {
        FbxVector4 vertex = fbxMesh->GetControlPointAt(i);
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
        node->vertices->GetLayout().Append({ ElementType::Texture2D });
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
        node->vertices->GetLayout().Append({ ElementType::Normal });
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
        int materialCount = fbxMesh->GetNode()->GetMaterialCount();
        if (materialCount != 0)
        {
            int materialIndex = fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(i);
            //node->textureFilenames.resize(materialCount);
            FbxSurfaceMaterial* material = fbxMesh->GetNode()->GetMaterial(materialIndex);
            if (material)
            {
                FbxProperty diffuseProp = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
                if (diffuseProp.GetSrcObjectCount<FbxTexture>() > 0)
                {
                    node->vertexType = node->vertexType | Diffuse;
                    GetMaterialTexture(diffuseProp, node->diffuse, uniqueTextureIndex);
                }
                

                FbxProperty specularProp = material->FindProperty(FbxSurfaceMaterial::sSpecular);
                if (specularProp.GetSrcObjectCount<FbxTexture>() > 0)
                {
                    int temp = 0;
                    GetMaterialTexture(specularProp, node->specular, temp);
                }
                

                //FbxProperty emissiveProp = material->FindProperty(FbxSurfaceMaterial::sEmissive);
                //GetMaterialTexture(emissiveProp, node, uniqueTextureIndex);

                FbxProperty normalProp = material->FindProperty(FbxSurfaceMaterial::sNormalMap);
                if (normalProp.GetSrcObjectCount<FbxTexture>() > 0)
                {
                    int temp = 0;
                    GetMaterialTexture(normalProp, node->normalMap, temp);
                }
              
                //FbxProperty reflectionProp = material->FindProperty(FbxSurfaceMaterial::sReflection);
                //GetMaterialTexture(reflectionProp, node, uniqueTextureIndex);

                //FbxProperty bumpProp = material->FindProperty(FbxSurfaceMaterial::sBump);
                //GetMaterialTexture(bumpProp, node, uniqueTextureIndex);
            }
        }
        

        assert(fbxMesh->GetPolygonSize(i) == 3);

        auto firstIndex = fbxMesh->GetPolygonVertex(i, 0);
        auto secondIndex = fbxMesh->GetPolygonVertex(i, 1);
        auto thirdIndex = fbxMesh->GetPolygonVertex(i, 2);

        node->vertices->Store(positions[firstIndex]);
        node->vertices->Store(DirectX::XMFLOAT3(texCoords[i * 3].x, (1.0f - texCoords[i * 3].y), uniqueTextureIndex));
        node->vertices->Store(normals[i*3]);
        node->vertices->Store(positions[secondIndex]);
        node->vertices->Store(DirectX::XMFLOAT3(texCoords[i * 3 + 1].x, (1.0f - texCoords[i * 3 + 1].y), uniqueTextureIndex));
        node->vertices->Store(normals[i*3+1]);
        node->vertices->Store(positions[thirdIndex]);
        node->vertices->Store(DirectX::XMFLOAT3(texCoords[i * 3 + 2].x, (1.0f - texCoords[i * 3 + 2].y), uniqueTextureIndex));
        node->vertices->Store(normals[i*3+2]);

        node->indices.push_back(i * 3);
        node->indices.push_back(i * 3 + 1);
        node->indices.push_back(i * 3 + 2);
    }

}

void FbxManagerWrapper::GetMaterialTexture(const FbxProperty& prop, std::vector<const wchar_t *>& texfilenames, int& uniqueTextureIndex)
{


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

                auto it = std::find_if(texfilenames.begin(), texfilenames.end(), [wideString](const wchar_t* str) {
                    return wcscmp(str, wideString) == 0;
                    });
                if (it == texfilenames.end())
                {
                    texfilenames.push_back(wideString);
                    uniqueTextureIndex = texfilenames.size();
                }
                else
                {
                    uniqueTextureIndex = std::distance(texfilenames.begin(), it);

                }
            }
        }
    }
}


