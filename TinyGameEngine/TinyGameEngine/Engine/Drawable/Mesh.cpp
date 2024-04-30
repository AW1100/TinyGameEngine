#include "Mesh.h"
#include "../Bindable/BindableHeader.h"
#include "SceneBindables.h"

#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Util/FbxManagerWrapper.h"
#include "Model.h"

Mesh::Mesh(Graphics& gfx, std::shared_ptr<MeshNode> node, DirectX::XMFLOAT3 trans, MeshType type, const std::string& n)
{
    translation = trans;
    //auto key = GenerateUID<VertexBuffer>(n);
    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<VertexBuffer>(n)), [&]()-> std::shared_ptr<Bindable>{
        return std::make_shared<VertexBuffer>(gfx, node->vertices->GetContents(), node->vertices->GetStride());
        }));
    //AddBind(std::make_unique<VertexBuffer>(gfx, node->vertices));

    auto pvs = SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<VertexShader>(n)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<VertexShader>(gfx, L"PhongVS.cso");
        });
    auto sp = std::dynamic_pointer_cast<VertexShader>(pvs);
    auto pvsbc = sp->GetBytecode();
    AddBind(std::move(sp));

    if (type == MeshType::Anima_Character)
    {
        AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<PixelShader>("ToonPS.cso")), [&]()-> std::shared_ptr<Bindable> {
            return std::make_shared<PixelShader>(gfx, L"ToonPS.cso");
            }));
    }
    else
    {
        AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<PixelShader>("BlinnPhongPS.cso")), [&]()-> std::shared_ptr<Bindable> {
            return std::make_shared<PixelShader>(gfx, L"BlinnPhongPS.cso");
            }));
    }

    AddIndexBuffer(std::dynamic_pointer_cast<IndexBuffer>(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<IndexBuffer>(n)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<IndexBuffer>(gfx, node->indices);
        })));
    
    std::vector<const wchar_t*> texFiles;
    if (node->diffuse.size() > 0)
    {
        texFiles.insert(texFiles.end(), node->diffuse.begin(), node->diffuse.end());
    }
    if (node->specular.size() > 0)
    {
        texFiles.insert(texFiles.end(), node->specular.begin(), node->specular.end());
    }
    if (texFiles.size() > 0)
    {
        AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Texture>(n)), [&]()-> std::shared_ptr<Bindable> {
            return std::make_shared<Texture>(gfx, node->diffuse);
            }));
    }
    
    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Sampler>()), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<Sampler>(gfx);
        }));

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Rasterizer>()), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<Rasterizer>(gfx);
        }));

    std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
    unsigned int offset = 0;
    auto& elems = node->vertices->GetLayout().elements;
    for (int i = 0; i < elems.size(); i++)
    {
        ied.push_back({ elems[i].semantic,0,DXGI_FORMAT_R32G32B32_FLOAT,0,offset,D3D11_INPUT_PER_VERTEX_DATA ,0 });
        offset += elems[i].sizeInByte;
    }

    /*const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
    {
        { "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "TexCoord",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
        { "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
    };*/
    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<InputLayout>()), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<InputLayout>(gfx, ied, pvsbc);
        }));

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Topology>()), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        }));

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<TransformConstantBuffer>(n)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<TransformConstantBuffer>(gfx, *this, 0);
        }));
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

