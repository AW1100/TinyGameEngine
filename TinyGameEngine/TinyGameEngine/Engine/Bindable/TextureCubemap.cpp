#include "TextureCubemap.h"
#include "DirectXTK\WICTextureLoader.h"
#include "DirectXTex\DirectXTex.h"
#include "..\Exception\Exception.h"

#include <vector>

TextureCubemap::TextureCubemap(Graphics& gfx, const std::vector<const wchar_t*>& filepaths, UINT slot)
	:slot(slot)
{
    //HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    Microsoft::WRL::ComPtr<ID3D11Resource> firstResource;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> firstTextureView;
    //hr = DirectX::CreateWICTextureFromFile(GetDevice(gfx), GetContext(gfx), filepaths[0], firstResource.GetAddressOf(), firstTextureView.GetAddressOf());
    HRESULT hr = DirectX::CreateWICTextureFromFileEx(
        GetDevice(gfx),               // Pointer to the Direct3D device
        GetContext(gfx),              // Pointer to the device context
        filepaths[0], // Path to the texture file
        0,                    // Maximum size for the texture (0 means no resizing)
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE, // Bind flags
        0,                    // CPU access flags
        0, // Misc. flags
        DirectX::WIC_LOADER_DEFAULT,       // Load flags
        firstResource.GetAddressOf(),            // Address of the resource pointer
        firstTextureView.GetAddressOf()                 // Address of the SRV pointer
    );
    if (FAILED(hr)) return; // Handle error

    // Get the description from the first loaded texture
    D3D11_TEXTURE2D_DESC textureDesc = {};
    Microsoft::WRL::ComPtr<ID3D11Texture2D> firstTexture;
    hr = firstResource.As(&firstTexture);
    if (FAILED(hr)) return;
    firstTexture->GetDesc(&textureDesc);

    // Modify the texture description to create a texture array
    textureDesc.ArraySize = static_cast<UINT>(filepaths.size());
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    textureDesc.MipLevels = 1;

    // Create the Texture2DArray
    Microsoft::WRL::ComPtr<ID3D11Texture2D> textureArray;
    DX::ThrowIfFailed(GetDevice(gfx)->CreateTexture2D(&textureDesc, nullptr, &textureArray));
    if (FAILED(hr)) return; // Handle error

    // Copy the first texture into the first slice of the texture array
    //GetContext(gfx)->CopyResource(textureArray.Get(), firstTexture.Get());
    auto d1 = D3D11CalcSubresource(0, 0, textureDesc.MipLevels);
    GetContext(gfx)->CopySubresourceRegion(textureArray.Get(), D3D11CalcSubresource(0, 0, textureDesc.MipLevels), 0, 0, 0, firstTexture.Get(), 0, nullptr);
    //GetContext(gfx)->UpdateSubresource()

    // Load the rest of the textures
    for (int i = 1; i < filepaths.size(); i++) {
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        //hr = DirectX::CreateWICTextureFromFile(GetDevice(gfx), nullptr, filepaths[i], resource.GetAddressOf(), nullptr);
        //if (FAILED(hr)) continue; // Handle error

        DirectX::ScratchImage* image_ptr;
        DirectX::ScratchImage image;
        DirectX::ScratchImage convertedImage;
        DirectX::ScratchImage resizedImage;
        image_ptr = &image;
        hr = DirectX::LoadFromWICFile(filepaths[i], DirectX::WIC_FLAGS_NONE, nullptr, image);
        auto& im = image_ptr->GetMetadata();
        if (im.format != textureDesc.Format)
        {
            DX::ThrowIfFailed(DirectX::Convert(image_ptr->GetImages(), image_ptr->GetImageCount(), image_ptr->GetMetadata(), textureDesc.Format, DirectX::TEX_FILTER_DEFAULT, 0.5, convertedImage));
            image_ptr = &convertedImage;
        }
        if (im.width != textureDesc.Width || im.height != textureDesc.Height)
        {
            DX::ThrowIfFailed(DirectX::Resize(image_ptr->GetImages(), image_ptr->GetImageCount(), image_ptr->GetMetadata(), textureDesc.Width, textureDesc.Height, DirectX::TEX_FILTER_DEFAULT, resizedImage));
            image_ptr = &resizedImage;
        }

        HRESULT hr = DirectX::CreateTexture(
            GetDevice(gfx), // ID3D11Device
            image_ptr->GetImages(), // Pointer to the image data
            image_ptr->GetImageCount(), // Number of images
            image_ptr->GetMetadata(), // Image metadata
            &resource // Where to store the created resource
        );

        // Copy each texture into its corresponding slice
        Microsoft::WRL::ComPtr<ID3D11Texture2D> srcTexture;
        if (FAILED(resource.As(&srcTexture))) {
            return;
        }
        D3D11_TEXTURE2D_DESC textureDesc1 = {};
        srcTexture->GetDesc(&textureDesc1);
        auto d2 = D3D11CalcSubresource(0, i, textureDesc.MipLevels);
        auto d3 = textureDesc.MipLevels;

        GetContext(gfx)->CopySubresourceRegion(textureArray.Get(), D3D11CalcSubresource(0, i, textureDesc.MipLevels), 0, 0, 0, srcTexture.Get(), 0, nullptr);
    }

    // Create a shader resource view for the texture array
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureArraySRV;
    hr = GetDevice(gfx)->CreateShaderResourceView(textureArray.Get(), &srvDesc, &textureArraySRV);
    if (FAILED(hr)) return; // Handle error

    // Store the SRV for use in rendering
    pTextureViews.push_back(textureArraySRV);

    //CoUninitialize();
}

TextureCubemap::~TextureCubemap()
{
}

void TextureCubemap::Bind(Graphics& gfx)
{
    if (!pTextureViews.empty()) {
        // Prepare an array of shader resource view pointers for binding
        std::vector<ID3D11ShaderResourceView*> srvs(pTextureViews.size());
        for (size_t i = 0; i < pTextureViews.size(); ++i) {
            srvs[i] = pTextureViews[i].Get(); // Extract the raw pointer
        }

        // Bind the texture views to the pixel shader (or another shader stage as needed)
        GetContext(gfx)->PSSetShaderResources(slot, static_cast<UINT>(srvs.size()), srvs.data());
    }
}
