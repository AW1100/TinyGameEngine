#pragma once
#include "Bindable.h"

#include <vector>

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned int>& indices);
	void Bind(Graphics& gfx) override;
	UINT GetCount() const;

protected:
	UINT count;
	ComPtr<ID3D11Buffer> pIndexBuffer;
};