//#include "Box.h"
//#include "../Bindable/BindableHeader.h"
//
//#include <sstream>
//
//Box::Box(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist, std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist, std::uniform_real_distribution<float>& rdist)
//    :r(rdist(rng)),
//    droll(ddist(rng)),
//    dpitch(ddist(rng)),
//    dyaw(ddist(rng)),
//    dphi(odist(rng)),
//    dtheta(odist(rng)),
//    dchi(odist(rng)),
//    chi(adist(rng)),
//    theta(adist(rng)),
//    phi(adist(rng))
//{
//    if (!IsStaticInitialized())
//    {
//        struct Vertex
//        {
//            struct
//            {
//                float x;
//                float y;
//                float z;
//            }pos;
//        };
//
//        std::vector<Vertex> vertices =
//        {
//            { -1.0f,-1.0f,-1.0f },
//            { 1.0f,-1.0f,-1.0f },
//            { -1.0f,1.0f,-1.0f },
//            { 1.0f,1.0f,-1.0f },
//            { -1.0f,-1.0f,1.0f },
//            { 1.0f,-1.0f,1.0f },
//            { -1.0f,1.0f,1.0f },
//            { 1.0f,1.0f,1.0f },
//        };
//        AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
//
//        auto pvs = std::make_unique<VertexShader>(gfx, L"ColorCubeVS.cso");
//        auto pvsbc = pvs->GetBytecode();
//        AddStaticBind(std::move(pvs));
//
//        AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorCubePS.cso"));
//
//        const std::vector<unsigned int> indices =
//        {
//            0,2,1, 2,3,1,
//            1,3,5, 3,7,5,
//            2,6,3, 3,6,7,
//            4,5,7, 4,7,6,
//            0,4,2, 2,4,6,
//            0,1,4, 1,5,4
//        };
//
//        AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));
//
//        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
//        {
//            { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
//        };
//        AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
//
//        AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
//    }
//    else
//    {
//        SetIndexBufferFromStatic();
//    }
//    
//    AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this, 0));
//}
//
//Box::~Box()
//{
//    /*std::ostringstream oss;
//    oss << "Box Destructor Called." << "\n";
//    OutputDebugStringA(oss.str().c_str());*/
//}
//
//void Box::Update(float dt)
//{
//    roll += droll * dt;
//    pitch += dpitch * dt;
//    yaw += dyaw * dt;
//    theta += dtheta * dt;
//    phi += dphi * dt;
//    chi += dchi * dt;
//}
//
//DirectX::XMMATRIX Box::GetModelMatrix() const
//{
//    return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
//        DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
//        DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
//}
