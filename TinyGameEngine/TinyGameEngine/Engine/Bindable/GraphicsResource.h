#pragma once
#include "../Graphics/Graphics.h"
#include "../Exception/Exception.h"

class GraphicsResource
{
protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx);
	static ID3D11Device* GetDevice(Graphics& gfx);
};