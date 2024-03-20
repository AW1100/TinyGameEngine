#include "Texture.h"
#include "DirectXTK\WICTextureLoader.h"

Texture::Texture(Graphics& gfx, const wchar_t* filepath)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = DirectX::CreateWICTextureFromFile(GetDevice(gfx), GetContext(gfx), filepath, reinterpret_cast<ID3D11Resource**>(texture.GetAddressOf()), textureView.GetAddressOf());

	D3D11_TEXTURE2D_DESC textureDesc = {};
	texture->GetDesc(&textureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GetDevice(gfx)->CreateShaderResourceView(texture.Get(), &srvDesc, &pTextureView);
}

void Texture::Bind(Graphics& gfx)
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}
