#include "FBXManager.h"


FBXManager::FBXManager()
{
    lSdkManager = FbxManager::Create();

    // Create the IO settings object.
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
}

FBXManager::~FBXManager()
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

std::shared_ptr<FbxScene> FBXManager::ReadFromFilepath(const char* filepath)
{
    lImporter = FbxImporter::Create(lSdkManager, "");

    // Use the first argument as the filename for the importer.
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

void FBXManager::LoadMeshData(FbxNode* rootNode, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<UV>& uvs)
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
                    UV tex;
                    tex.uv.u = uv[0];
                    tex.uv.v = uv[1];
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
        for (int i = 0; i < node->GetChildCount(); i++) {
            selfRef(node->GetChild(i), selfRef); // Recursive call
        }
        };

    processRecursive(rootNode, processRecursive);
}
