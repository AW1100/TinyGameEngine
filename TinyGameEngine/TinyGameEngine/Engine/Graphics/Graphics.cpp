#include "Graphics.h"
#include "../Exception/WindowException.h"
#include "../Constants.h"

#include <d3d11sdklayers.h>

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

#include "../Light/PointLight.h"
#include "../Drawable/Scene.h"

Graphics::Graphics(HWND hWnd)
{
	CreateDevice();
	CreateSwapChain(hWnd);
	CreateShadowMapResource();

	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

Graphics::~Graphics()
{
	if (pDebug != nullptr)
	{
		pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		pDebug = nullptr;
	}
}

void Graphics::CreateDevice()
{
	DX::ThrowIfFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &pDevice, nullptr, &pContext));
}

void Graphics::CreateSwapChain(HWND hWnd)
{
	UINT msaaQualityLevels = 0;
	DX::ThrowIfFailed(pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_B8G8R8A8_UNORM, 
		4,                     
		&msaaQualityLevels)); 
	UINT msaaQuality = msaaQualityLevels - 1;

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = SAMPLES;
	sd.SampleDesc.Quality = msaaQuality;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	DX::ThrowIfFailed(pDevice.As(&dxgiDevice));

	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	DX::ThrowIfFailed(dxgiDevice->GetAdapter(&adapter));

	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
	DX::ThrowIfFailed(adapter->GetParent(__uuidof(IDXGIFactory), &factory));

	DX::ThrowIfFailed(factory->CreateSwapChain(pDevice.Get(), &sd, &pSwap));

#ifdef _DEBUG
	pDevice->QueryInterface(IID_PPV_ARGS(&pDebug));
#endif
	ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	DX::ThrowIfFailed(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	));

	// create depth stensil texture
	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = WIDTH;
	descDepth.Height = HEIGHT;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = SAMPLES;
	descDepth.SampleDesc.Quality = msaaQuality;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DX::ThrowIfFailed(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0u;
	DX::ThrowIfFailed(pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDSV
	));

	// bind depth stensil view to OM
	// pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	for (int i = 0; i < 6; i++)
	{
		pContext->ClearRenderTargetView(pShadowCubeRTV.Get(), color);
		pContext->ClearDepthStencilView(pShadowCubeDSV[i].Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	}
	
}

void Graphics::Draw(UINT count)
{
	pContext->Draw(count, 0u);
}

void Graphics::DrawIndexed(UINT count)
{
	pContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::DrawTestTriangle()
{
	struct Vertex
	{
		float x;
		float y;
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
	const Vertex vertices[] =
	{
		{0.0f, 0.5f, 255,0,0},
		{0.5f, -0.5f, 0,255,0},
		{-0.5f, -0.5f, 0,0,255},
		{0.0f, -1.0f, 255,255,0}
	};
	ComPtr<ID3D11Buffer> pVertextBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	pDevice->CreateBuffer(&bd, &sd, &pVertextBuffer);
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertextBuffer.GetAddressOf(), &stride, &offset);

	const unsigned short indices[] =
	{
		0,1,2,
		2,1,3,
	};
	ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


	ComPtr<ID3DBlob> pBlob;
	ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

	ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	pContext->IASetInputLayout(pInputLayout.Get());

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

void Graphics::SetProjection(DirectX::XMMATRIX proj)
{
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const
{
	return projection;
}

void Graphics::BeginImgui(float &sf)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow(); // Show demo window! :)
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &sf, 0.0f, 4.0f);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();
	
}

void Graphics::RenderImgui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Graphics::SetCamera(DirectX::FXMMATRIX cam)
{
	camera = cam;
}

DirectX::XMMATRIX Graphics::GetCamera() const
{
	return camera;
}

void Graphics::SetShadowPassRT()
{
	/*ID3D11RenderTargetView* rtv[6] = { 
		pShadowCubeRTV[0].Get(),
		pShadowCubeRTV[1].Get(),
		pShadowCubeRTV[2].Get(),
		pShadowCubeRTV[3].Get(),
		pShadowCubeRTV[4].Get(),
		pShadowCubeRTV[5].Get() };
	ID3D11DepthStencilView* dsv[6] = {
		pShadowCubeDSV[0].Get(),
		pShadowCubeDSV[1].Get(),
		pShadowCubeDSV[2].Get(),
		pShadowCubeDSV[3].Get(),
		pShadowCubeDSV[4].Get(),
		pShadowCubeDSV[5].Get(),
	};
	pContext->OMSetRenderTargets(1u, rtv, nullptr);
	for (int i = 0; i < 6; ++i)
	{
		pContext->OMSetRenderTargets(1u, pShadowCubeRTV[i].GetAddressOf(), pShadowCubeDSV[i].Get());
	}

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = 512;
	vp.Height = 512;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	pContext->RSSetViewports(1, &vp);*/
}

void Graphics::SetShadowPassRT(int index)
{
	pContext->OMSetRenderTargets(1u, pShadowCubeRTV.GetAddressOf(), pShadowCubeDSV[index].Get());

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = 1024.0f;
	vp.Height = 1024.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	pContext->RSSetViewports(1, &vp);
}

void Graphics::SetBasePassRT()
{
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);
}

void Graphics::BindShadowMapToPixelShader()
{
	ID3D11ShaderResourceView* nullSRVs[2] = { nullptr,nullptr };
	pContext->PSSetShaderResources(0, 2, nullSRVs); // Unbind from pixel shader slot 0
	pContext->PSSetShaderResources(1, 1, pShadowDepthSRV.GetAddressOf());
}

void Graphics::UnbindShadowMapToPixelShader()
{
	ID3D11ShaderResourceView* nullSRVs[1] = { nullptr };
	pContext->PSSetShaderResources(1, 1, nullSRVs); // Unbind from pixel shader slot 0
}

void Graphics::SetShadowMapConstantBuffer(DirectX::XMMATRIX& matrix, UINT s)
{
	VertexConstantBuffer<DirectX::XMMATRIX> vcb(*this, matrix);
	vcb.slot = s;
	vcb.Bind(*this);
}

void Graphics::UnbindRenderTarget()
{
	ID3D11ShaderResourceView* nullSRVs[1] = { nullptr };
	pContext->PSSetShaderResources(0, 1, nullSRVs); // Unbind from pixel shader slot 0
	pContext->VSSetShaderResources(0, 1, nullSRVs); // Unbind from vertex shader slot 0
	// Repeat for other shader stages if necessary (geometry shader, compute shader, etc.)

	// Unbind any UAVs if used
	ID3D11UnorderedAccessView* nullUAVs[1] = { nullptr };
	pContext->CSSetUnorderedAccessViews(0, 1, nullUAVs, nullptr);
}

void Graphics::CreateShadowMapResource()
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = 1024u; // Ensure WIDTH / 2 is an integer
	texDesc.Height = 1024u; // Ensure HEIGHT / 2 is an integer
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6; // 6 faces of the cube
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	DX::ThrowIfFailed(pDevice->CreateTexture2D(&texDesc, nullptr, &pShadowDepth));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 1;

	
	for (int i = 0; i < 6; ++i)
	{
		dsvDesc.Texture2DArray.FirstArraySlice = i;
		DX::ThrowIfFailed(pDevice->CreateDepthStencilView(pShadowDepth.Get(), &dsvDesc, &pShadowCubeDSV[i]));
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = 1;

	DX::ThrowIfFailed(pDevice->CreateShaderResourceView(pShadowDepth.Get(), &srvDesc, &pShadowDepthSRV));

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 1024u;
	textureDesc.Height = 1024u;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // Or any other format you need
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = 0;

	DX::ThrowIfFailed(pDevice->CreateTexture2D(&textureDesc, nullptr, &pShadowRT));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.MipSlice = 0;
	rtvDesc.Texture2DArray.ArraySize = 1;

	DX::ThrowIfFailed(pDevice->CreateRenderTargetView(pShadowRT.Get(), &rtvDesc, &pShadowCubeRTV));

	/*D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = 1;

	DX::ThrowIfFailed(pDevice->CreateShaderResourceView(pShadowRT.Get(), &srvDesc, &pShadowSRV));*/
}

void Graphics::ShadowPass(PointLight& light)
{
	


}

void Graphics::UnbindGeometryShader()
{
	pContext->GSSetShader(nullptr, nullptr, 0);
}

void Graphics::ClearRenderTarget()
{
	const float color[] = { 0.1f,0.1f,0.1f,1.0f };
	pContext->ClearRenderTargetView(pShadowCubeRTV.Get(), color);
}
