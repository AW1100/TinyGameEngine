#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <span>
#include <DirectXMath.h>
#include <memory>

using namespace Microsoft::WRL;

class Graphics
{
	friend class GraphicsResource;
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void CreateDevice();
	void CreateSwapChain(HWND hWnd);
	void EndFrame();
	void ClearBuffer(float red, float green, float blue);
	void Draw(UINT count);
	void DrawIndexed(UINT count);
	//void DrawTestTriangle();
	void SetProjection(DirectX::XMMATRIX proj);
	DirectX::XMMATRIX GetProjection() const;
	void BeginImgui(float &sf);
	void RenderImgui();
	void SetCamera(DirectX::FXMMATRIX cam);
	DirectX::XMMATRIX GetCamera() const;

	void SetShadowPassDepthStencil(int index);
	void SetBasePassRT();
	void BindShadowMapToPixelShader();
	void UnbindShadowMapToPixelShader();
	void SetShadowMapConstantBuffer(DirectX::XMMATRIX& matrix, UINT s);
	void UnbindRenderTarget();

	void CreateShadowPassResource();
	void UnbindGeometryShader();
	void ClearRenderTarget();

	void CreatePostProcessingResource();
	void SetPostProcessingRT();
	void BindPostRTToPixelShader();
	void SetConvRT();
	void BindConvRTToPixelShader();

private:
	ComPtr<ID3D11Texture2D> pShadowDepth;
	ComPtr<ID3D11DepthStencilView> pShadowCubeDSV[6];
	ComPtr<ID3D11ShaderResourceView> pShadowDepthSRV;

	ComPtr<ID3D11Texture2D> pShadowRT;
	ComPtr<ID3D11RenderTargetView> pShadowCubeRTV;
	ComPtr<ID3D11ShaderResourceView> pShadowSRV;

	ComPtr< ID3D11Texture2D> pPostRT;
	ComPtr<ID3D11RenderTargetView> pPostRTV;
	ComPtr<ID3D11ShaderResourceView> pPostSRV;
	ComPtr< ID3D11Texture2D> pConvRT;
	ComPtr<ID3D11RenderTargetView> pConvRTV;
	ComPtr<ID3D11ShaderResourceView> pConvSRV;

	ComPtr<ID3D11Device> pDevice = nullptr;
	ComPtr<IDXGISwapChain> pSwap = nullptr;
	ComPtr<ID3D11DeviceContext> pContext = nullptr;
	ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	//ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	std::unique_ptr<class DepthStencil> pDepthStencil;

#ifdef _DEBUG 
	ComPtr<ID3D11Debug> pDebug;
#endif

	DirectX::XMMATRIX camera;
	DirectX::XMMATRIX projection;
};