#include "Model.h"
#include "../Bindable/BindableHeader.h"

#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Util/FBXManager.h"

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
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<UV> texcoords;

        FBXManager fbxm;
        auto lScene = fbxm.ReadFromFilepath("F:/3DModels/fbx/kafka.fbx");
        fbxm.LoadMeshData(lScene->GetRootNode(),vertices,indices, texcoords);

        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

        AddStaticBind(std::make_unique<Texture>(gfx, L"F:/TinyGameEngine/Assets/01.jpg"));
        AddStaticBind(std::make_unique<Sampler>(gfx));

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
