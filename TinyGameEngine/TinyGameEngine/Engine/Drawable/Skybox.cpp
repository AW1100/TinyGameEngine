#include "SkyBox.h"
#include "../Bindable/BindableHeader.h"
#include <string>

Skybox::Skybox(Graphics& gfx)
{
	std::vector<const wchar_t*> texfiles =
	{
		L"F:\\TinyGameEngine\\Assets\\SpaceBox\\0.png",
		L"F:\\TinyGameEngine\\Assets\\SpaceBox\\1.png",
		L"F:\\TinyGameEngine\\Assets\\SpaceBox\\2.png",
		L"F:\\TinyGameEngine\\Assets\\SpaceBox\\3.png",
		L"F:\\TinyGameEngine\\Assets\\SpaceBox\\4.png",
		L"F:\\TinyGameEngine\\Assets\\SpaceBox\\5.png"
	};

	AddBind(std::make_shared<TextureCubemap>(gfx, texfiles));

	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		}pos;
	};

	std::vector<Vertex> vertices =
	{
		{ -1.0f, -1.0f, -1.0f },  // 0
		{  1.0f, -1.0f, -1.0f },  // 1
		{ -1.0f,  1.0f, -1.0f },  // 2
		{  1.0f,  1.0f, -1.0f },  // 3
		{ -1.0f, -1.0f,  1.0f },  // 4
		{  1.0f, -1.0f,  1.0f },  // 5
		{ -1.0f,  1.0f,  1.0f },  // 6
		{  1.0f,  1.0f,  1.0f },  // 7
	};

	const std::vector<unsigned int> indices =
	{
		// Front face
		0, 1, 2, 2, 1, 3,
		// Right face
		1, 5, 3, 3, 5, 7,
		// Back face
		5, 4, 7, 7, 4, 6,
		// Left face
		4, 0, 6, 6, 0, 2,
		// Top face
		2, 3, 6, 6, 3, 7,
		// Bottom face
		4, 5, 0, 0, 5, 1,
	};

	AddBind(std::make_shared<VertexBuffer>(gfx, vertices));

	auto pvs = std::make_shared<VertexShader>(gfx, L"SkyBoxVS.cso");
	auto pvsbc = pvs->GetBytecode();
	AddBind(std::move(pvs));

	AddBind(std::make_shared<PixelShader>(gfx, L"SkyBoxPS.cso"));

	AddIndexBuffer(std::make_shared<IndexBuffer>(gfx, indices));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddBind(std::make_shared<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_shared<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<TransformConstantBuffer>(gfx, *this, 0));

	AddBind(std::make_shared<Stencil>(gfx, Stencil::Mode::DepthLessEqual));

	AddBind(std::make_shared<Sampler>(gfx));

	AddBind(std::make_shared<Rasterizer>(gfx, false));
}

Skybox::~Skybox()
{
}

DirectX::XMMATRIX Skybox::GetModelMatrix() const
{
	return DirectX::XMMatrixScaling(2.0f,2.0f,2.0f);
}
