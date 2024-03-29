#include "Graphics.h"
#include "../Exception/WindowException.h"
#include "../Constants.h"

#include <d3d11sdklayers.h>

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
{
	CreateDevice();
	CreateSwapChain(hWnd);

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

	// create depth stensil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ComPtr<ID3D11DepthStencilState> pDSState;
	DX::ThrowIfFailed(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stensil texture
	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = WIDTH;
	descDepth.Height = HEIGHT;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = SAMPLES;
	descDepth.SampleDesc.Quality = msaaQuality;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DX::ThrowIfFailed(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0u;
	DX::ThrowIfFailed(pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDSV
	));

	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

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
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
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
