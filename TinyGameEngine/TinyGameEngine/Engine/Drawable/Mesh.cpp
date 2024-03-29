#include "Mesh.h"
#include "../Bindable/BindableHeader.h"

#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Util/FbxManagerWrapper.h"
#include "Model.h"

Mesh::Mesh(Graphics& gfx, std::shared_ptr<TreeNode> node)
{
    if (!IsStaticInitialized())
    {
        node->textureFilenames.push_back(L"F:/3DModels/face.png");
        AddStaticBind(std::make_unique<VertexBuffer>(gfx, node->vertices));

        auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
        auto pvsbc = pvs->GetBytecode();
        AddStaticBind(std::move(pvs));

        AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, node->indices));

        AddStaticBind(std::make_unique<Texture>(gfx, node->textureFilenames));
        AddStaticBind(std::make_unique<Sampler>(gfx));

        AddStaticBind(std::make_unique<Rasterizer>(gfx));

        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
        {
            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "TexCoord",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
        };
        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        SetIndexBufferFromStatic();
    }

    AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this, 0));
    PixelConstantBuffer<DirectX::XMMATRIX> pcb(gfx, 1);
    pcb.Update(gfx, GetModelMatrix());

}

Mesh::~Mesh()
{
    /*std::ostringstream oss;
    oss << "Box Destructor Called." << "\n";
    OutputDebugStringA(oss.str().c_str());*/
}

void Mesh::Update(float dt)
{
    roll += droll * dt;
    pitch += dpitch * dt;
    yaw += dyaw * dt;
    theta += dtheta * dt;
    phi += dphi * dt;
    chi += dchi * dt;
}

DirectX::XMMATRIX Mesh::GetModelMatrix() const
{
    /*return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
        DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
        DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);*/
    return DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XMConvertToRadians(180.0f))*
        DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f)*
        DirectX::XMMatrixTranslation(0.0f, -1.0f, 0.0f);
}

