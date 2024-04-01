#include "Mesh.h"
#include "../Bindable/BindableHeader.h"

#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Util/FbxManagerWrapper.h"
#include "Model.h"

Mesh::Mesh(Graphics& gfx, std::shared_ptr<TreeNode> node, DirectX::XMFLOAT3 trans)
{
    translation = trans;
    //node->textureFilenames.push_back(L"F:/3DModels/face.png");
    AddBind(std::make_unique<VertexBuffer>(gfx, node->vertices));

    auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
    auto pvsbc = pvs->GetBytecode();
    AddBind(std::move(pvs));

    AddBind(std::make_unique<PixelShader>(gfx, L"BlinnPhongPS.cso"));

    AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, node->indices));

    if (node->textureFilenames.size() > 0)
    {
        AddBind(std::make_unique<Texture>(gfx, node->textureFilenames));
    }
    
    AddBind(std::make_unique<Sampler>(gfx));

    AddBind(std::make_unique<Rasterizer>(gfx));

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
    {
        { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "TexCoord",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
    };
    AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

    AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this, 0));
    /*PixelConstantBuffer<DirectX::XMMATRIX> pcb(gfx, 1);
    pcb.Update(gfx, GetModelMatrix());*/

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
    return //DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XMConvertToRadians(180.0f))*
        //DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f)*
        DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);
        //DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
}

