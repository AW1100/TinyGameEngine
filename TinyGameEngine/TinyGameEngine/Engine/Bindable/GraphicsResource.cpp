#include "GraphicsResource.h"

ID3D11DeviceContext* GraphicsResource::GetContext(Graphics& gfx)
{
    return gfx.pContext.Get();
}

ID3D11Device* GraphicsResource::GetDevice(Graphics& gfx)
{
    return gfx.pDevice.Get();
}