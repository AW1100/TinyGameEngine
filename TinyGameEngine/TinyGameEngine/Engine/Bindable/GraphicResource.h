#pragma once
#include "../Graphics/Graphics.h"

class GraphicResource
{
protected:
	static ID3D11DeviceContext* GetContext(Graphics& gfx);
	static ID3D11Device* GetDevice(Graphics& gfx);
};