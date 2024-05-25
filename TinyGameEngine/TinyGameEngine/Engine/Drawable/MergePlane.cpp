#include "MergePlane.h"
#include "../Bindable/BindableHeader.h"

MergePlane::MergePlane(Graphics& gfx)
{
    std::vector<DirectX::XMFLOAT3> vertices;
    vertices.emplace_back(-1, 1, 0);
    vertices.emplace_back(1, 1, 0);
    vertices.emplace_back(-1, -1, 0);
    vertices.emplace_back(1, -1, 0);

    std::vector<unsigned int> indices = { 0,1,2,1,3,2 };

    AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

    auto pvs = std::make_unique<VertexShader>(gfx, L"MergeVS.cso");
    auto pvsbc = pvs->GetBytecode();
    AddBind(std::move(pvs));

    AddBind(std::make_unique<PixelShader>(gfx, L"MergePS.cso"));

    AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
    {
        { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
    };
    AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

    AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this, 0));

    AddBind(std::make_unique<Stencil>(gfx, Stencil::Mode::Mask));

    AddBind(std::make_unique<Sampler>(gfx));
}

void MergePlane::Update(float dt)
{
}

DirectX::XMMATRIX MergePlane::GetModelMatrix() const
{
    return DirectX::XMMATRIX();
}
