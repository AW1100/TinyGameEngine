#include "GraphicResource.h"

ID3D11DeviceContext* GraphicResource::GetContext(Graphics& gfx)
{
    return gfx.pContext.Get();
}

ID3D11Device* GraphicResource::GetDevice(Graphics& gfx)
{
    return gfx.pDevice.Get();
}
