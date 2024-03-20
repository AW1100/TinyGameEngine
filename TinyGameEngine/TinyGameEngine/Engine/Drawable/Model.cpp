#include "Model.h"
#include "../Bindable/BindableHeader.h"

#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist)
    :r(),
    droll(),
    dpitch(),
    dyaw(),
    dphi(),
    dtheta(),
    dchi(),
    chi(),
    theta(),
    phi()
{
    if (!IsStaticInitialized())
    {
        struct Vertex
        {
            struct
            {
                float x;
                float y;
                float z;
            }pos;
            struct
            {
                float u;
                float v;
            }tex;
            struct
            {
                float x;
                float y;
                float z;
            }normal;

        };


        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile("F:/3DModels/fbx/kafka.fbx", aiProcess_OptimizeMeshes | aiProcess_ValidateDataStructure);

        // Loop through each mesh
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // Process each mesh
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[i];

            // Process each vertex
            for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
                Vertex vertex;

                // Position
                vertex.pos.x = mesh->mVertices[j].x;
                vertex.pos.y = mesh->mVertices[j].y;
                vertex.pos.z = mesh->mVertices[j].z;

                // Texture Coordinates
                if (mesh->mTextureCoords[0]) {
                    vertex.tex.u = mesh->mTextureCoords[0][j].x;
                    vertex.tex.v = mesh->mTextureCoords[0][j].y;
                }
                else {
                    vertex.tex.u = 0.0f;
                    vertex.tex.v = 0.0f;
                }

                // Normals
                if (mesh->HasNormals()) {
                    vertex.normal.x = mesh->mNormals[j].x;
                    vertex.normal.y = mesh->mNormals[j].y;
                    vertex.normal.z = mesh->mNormals[j].z;
                }

                vertices.push_back(vertex);
            }

            // Process indices
            for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
                aiFace face = mesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; k++) {
                    indices.push_back(face.mIndices[k]);
                }
            }

            
        }

        if (scene->HasMaterials())
        {
            for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
                aiMaterial* material = scene->mMaterials[i];
                int a = 0;
            }
        }

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        AddStaticBind(std::make_unique<Texture>(gfx, L"F:/TinyGameEngine/Assets/01.jpg"));
        AddStaticBind(std::make_unique<Sampler>(gfx));

        struct CB
        {
            struct
            {
                float r;
                float g;
                float b;
                float a;
            } face_colors[6];
        };

        const CB cbuf =
        {
            {
                { 1.0f,0.0f,1.0f },
                { 1.0f,0.0f,0.0f },
                { 0.0f,1.0f,0.0f },
                { 0.0f,0.0f,1.0f },
                { 1.0f,1.0f,0.0f },
                { 0.0f,1.0f,1.0f },
            }
        };
        AddStaticBind(std::make_unique<PixelConstantBuffer<CB>>(gfx, cbuf));
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
        };
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexBufferFromStatic();
    }

    AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));
}

Model::~Model()
{
    /*std::ostringstream oss;
    oss << "Box Destructor Called." << "\n";
    OutputDebugStringA(oss.str().c_str());*/
}

void Model::Update(float dt)
{
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
}

DirectX::XMMATRIX Model::GetTransformXM() const
{
    return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
        DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
        DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}

void Model::loadModel()
{

}
