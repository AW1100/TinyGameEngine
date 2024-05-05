#include "Mesh.h"
#include "../Bindable/BindableHeader.h"
#include "SceneBindables.h"

#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Util/FbxManagerWrapper.h"
#include "Model.h"
#include "../Util/Misc.h"

Mesh::Mesh(Graphics& gfx, std::shared_ptr<MeshNode> node, DirectX::XMFLOAT3 trans, MeshType type, const std::string& n, unsigned int vertexType, bool useOutline)
    :translation(trans), outlineEnabled(useOutline)
{
    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<VertexBuffer>(n)), [&]()-> std::shared_ptr<Bindable>{
        return std::make_shared<VertexBuffer>(gfx, node->vertices->GetContents(), node->vertices->GetStride());
        }));

    ShaderLookupTable table;
    
    auto& targetVS = table.GetVertexShaderfilename(type, vertexType);
    auto vs = StringToWideString(targetVS.c_str());

    auto pvs = SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<VertexShader>(targetVS)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<VertexShader>(gfx, vs);
        });
    auto sp = std::dynamic_pointer_cast<VertexShader>(pvs);
    auto pvsbc = sp->GetBytecode();
    AddBind(std::move(sp));

    auto& targetPS = table.GetPixelShaderfilename(type, vertexType);
    auto ps = StringToWideString(targetPS.c_str());

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<PixelShader>(targetPS)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<PixelShader>(gfx, ps);
        }));

    AddIndexBuffer(std::dynamic_pointer_cast<IndexBuffer>(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<IndexBuffer>(n)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<IndexBuffer>(gfx, node->indices);
        })));
    
    bool useAlpha = false;
    std::vector<const wchar_t*> texFiles;
    if (node->diffuse.size() > 0)
    {
        texFiles.insert(texFiles.end(), node->diffuse.begin(), node->diffuse.end());
    }
    if (node->specular.size() > 0)
    {
        texFiles.insert(texFiles.end(), node->specular.begin(), node->specular.end());
    }
    if (node->normalMap.size() > 0)
    {
        texFiles.insert(texFiles.end(), node->normalMap.begin(), node->normalMap.end());
    }
    if (texFiles.size() > 0)
    {
        AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Texture>(n)), [&]()-> std::shared_ptr<Bindable> {
            return std::make_shared<Texture>(gfx, texFiles, useAlpha);
            }));
    }
    
    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Sampler>()), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<Sampler>(gfx);
        }));

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Rasterizer>(useAlpha ? "None" : "Back")), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<Rasterizer>(gfx, useAlpha);
        }));

    std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
    unsigned int offset = 0;
    auto& elems = node->vertices->GetLayout().elements;
    for (int i = 0; i < elems.size(); i++)
    {
        ied.push_back({ elems[i].semantic,0,DXGI_FORMAT_R32G32B32_FLOAT,0,offset,D3D11_INPUT_PER_VERTEX_DATA ,0 });
        offset += elems[i].sizeInByte;
    }

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<InputLayout>(n)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<InputLayout>(gfx, ied, pvsbc);
        }));

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Topology>()), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        }));

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Blender>(n)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<Blender>(gfx, true);
        }));

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<TransformConstantBuffer>(n)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<TransformConstantBuffer>(gfx, *this, 0);
        }));

    AddBind(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Stencil>(n)), [&]()-> std::shared_ptr<Bindable> {
        return std::make_shared<Stencil>(gfx, useOutline ? Stencil::Mode::Write : Stencil::Mode::Off);
        }));


    if (useOutline)
    {
        // Outline
        outlineBinds.push_back(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<VertexBuffer>(n)), [&]()-> std::shared_ptr<Bindable> {
            return std::make_shared<VertexBuffer>(gfx, node->vertices->GetContents(), node->vertices->GetStride());
            }));

        sp = std::make_shared<VertexShader>(gfx, L"SolidVS.cso");
        pvsbc = sp->GetBytecode();
        outlineBinds.push_back(std::move(sp));
        outlineBinds.push_back(std::move(std::make_shared<PixelShader>(gfx, L"OutlinePS.cso")));
        outlineBinds.push_back(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<IndexBuffer>(n)), [&]()-> std::shared_ptr<Bindable> {
            return std::make_shared<IndexBuffer>(gfx, node->indices);
            }));
        outlineBinds.push_back(std::make_shared<InputLayout>(gfx, ied, pvsbc));
        outlineBinds.push_back(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<Topology>()), [&]()-> std::shared_ptr<Bindable> {
            return std::make_shared<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            }));
        outlineBinds.push_back(SceneBindables::GetInstance().GetBindable(std::move(GenerateUID<TransformConstantBuffer>(n)), [&]()-> std::shared_ptr<Bindable> {
            return std::make_shared<TransformConstantBuffer>(gfx, *this, 0);
            }));
        outlineBinds.push_back(std::make_shared<Stencil>(gfx, Stencil::Mode::Mask));
    }
    
}

Mesh::~Mesh()
{

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
    auto modelMatrix = //DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, DirectX::XMConvertToRadians(180.0f))*
        //DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f)*
        DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);
    //DirectX::XMMatrixScaling(0.01f, 0.01f, 0.01f);
    if (outlining)
    {
        modelMatrix = DirectX::XMMatrixTranslation(0.0f, -0.005f, 0.0f) * DirectX::XMMatrixScaling(1.01f, 1.01f, 1.01f) * modelMatrix;
    }
    return modelMatrix;
}

void Mesh::DrawOutline(Graphics& gfx)
{
    outlining = true;
    for (auto& bind : outlineBinds)
    {
        bind->Bind(gfx);
    }
    gfx.DrawIndexed(pIndexBuffer->GetCount());
    outlining = false;
}

