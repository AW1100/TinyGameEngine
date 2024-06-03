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
#include "../Bindable/BindableHeader.h"

Graphics::Graphics(HWND hWnd)
{
	CreateDevice();
	CreateSwapChain(hWnd);
	CreateShadowPassResource();
	CreatePostProcessingResource();

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
	sd.SampleDesc.Count = SAMPLES_COUNT;
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
	
	pDepthStencil = std::make_unique<DepthStencil>(*this, WINDOW_WIDTH, WINDOW_HEIGHT, SAMPLES_COUNT, msaaQuality);
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDepthStencil->Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	
	pShadowRT->Clear(*this, color);
	pShadowCube->Clear(*this);

	pPostRT->Clear(*this, color);
	pConvRT->Clear(*this, color);
}

void Graphics::Draw(UINT count)
{
	pContext->Draw(count, 0u);
}

void Graphics::DrawIndexed(UINT count)
{
	pContext->DrawIndexed(count, 0u, 0u);
}

//void Graphics::DrawTestTriangle()
//{
//	struct Vertex
//	{
//		float x;
//		float y;
//		unsigned char r;
//		unsigned char g;
//		unsigned char b;
//	};
//	const Vertex vertices[] =
//	{
//		{0.0f, 0.5f, 255,0,0},
//		{0.5f, -0.5f, 0,255,0},
//		{-0.5f, -0.5f, 0,0,255},
//		{0.0f, -1.0f, 255,255,0}
//	};
//	ComPtr<ID3D11Buffer> pVertextBuffer;
//	D3D11_BUFFER_DESC bd = {};
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.CPUAccessFlags = 0u;
//	bd.MiscFlags = 0u;
//	bd.ByteWidth = sizeof(vertices);
//	bd.StructureByteStride = sizeof(Vertex);
//	D3D11_SUBRESOURCE_DATA sd = {};
//	sd.pSysMem = vertices;
//	pDevice->CreateBuffer(&bd, &sd, &pVertextBuffer);
//	const UINT stride = sizeof(Vertex);
//	const UINT offset = 0u;
//	pContext->IASetVertexBuffers(0u, 1u, pVertextBuffer.GetAddressOf(), &stride, &offset);
//
//	const unsigned short indices[] =
//	{
//		0,1,2,
//		2,1,3,
//	};
//	ComPtr<ID3D11Buffer> pIndexBuffer;
//	D3D11_BUFFER_DESC ibd = {};
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.Usage = D3D11_USAGE_DEFAULT;
//	ibd.CPUAccessFlags = 0u;
//	ibd.MiscFlags = 0u;
//	ibd.ByteWidth = sizeof(indices);
//	ibd.StructureByteStride = sizeof(unsigned short);
//	D3D11_SUBRESOURCE_DATA isd = {};
//	isd.pSysMem = indices;
//	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
//	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
//
//
//	ComPtr<ID3DBlob> pBlob;
//	ComPtr<ID3D11PixelShader> pPixelShader;
//	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
//	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
//	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);
//
//	ComPtr<ID3D11VertexShader> pVertexShader;
//	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
//	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
//	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
//
//	ComPtr<ID3D11InputLayout> pInputLayout;
//	const D3D11_INPUT_ELEMENT_DESC ied[] =
//	{
//		{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
//		{"COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
//	};
//
//	pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
//	pContext->IASetInputLayout(pInputLayout.Get());
//
//	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
//
//	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	D3D11_VIEWPORT vp;
//	vp.Width = WIDTH;
//	vp.Height = HEIGHT;
//	vp.MinDepth = 0;
//	vp.MaxDepth = 1;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	pContext->RSSetViewports(1u, &vp);
//
//	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
//}

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

void Graphics::SetShadowPassDepthStencil(int index)
{
	pShadowRT->BindToOutputMerger(*this, pShadowCube->Get(index));

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
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDepthStencil->Get());

	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);
}

void Graphics::BindShadowMapToPixelShader()
{
	//ID3D11ShaderResourceView* nullSRVs[2] = { nullptr,nullptr };
	//pContext->PSSetShaderResources(0, 2, nullSRVs); // Unbind from pixel shader slot 0
	pShadowCube->BindToPixelShader(*this, 1u);
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

void Graphics::CreateShadowPassResource()
{
	pShadowCube = std::make_unique<DepthCubemap>(*this, 1024u);
	pShadowRT = std::make_unique<RenderTarget>(*this, 1024u, 1024u);
}

void Graphics::UnbindGeometryShader()
{
	pContext->GSSetShader(nullptr, nullptr, 0);
}

void Graphics::ClearRenderTarget()
{
	const float color[] = { 0.1f,0.1f,0.1f,1.0f };
	pContext->ClearRenderTargetView(pShadowRT->Get(), color);
}

void Graphics::CreatePostProcessingResource()
{
	pPostRT = std::make_unique<RenderTarget>(*this, WINDOW_WIDTH, WINDOW_HEIGHT);
	pConvRT = std::make_unique<RenderTarget>(*this, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Graphics::SetPostProcessingRT()
{
	pPostRT->BindToOutputMerger(*this);
}

void Graphics::BindPostRTToPixelShader()
{
	pPostRT->BindToPixelShader(*this, 0u);
}

void Graphics::SetConvRT()
{
	pConvRT->BindToOutputMerger(*this);
}

void Graphics::BindConvRTToPixelShader()
{
	pConvRT->BindToPixelShader(*this, 0u);
}


