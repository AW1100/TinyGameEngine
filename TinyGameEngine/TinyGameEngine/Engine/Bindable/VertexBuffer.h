#pragma once
#include "Bindable.h"

#include <vector>

class VertexBuffer : public Bindable
{
public:
	template<typename T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
		:stride(sizeof(T))
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0U;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(T) * vertices.size());
		bd.StructureByteStride = sizeof(T);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer);
	}

	void Bind(Graphics& gfx) override;

protected:
	UINT stride;
	ComPtr<ID3D11Buffer> pVertexBuffer;
};