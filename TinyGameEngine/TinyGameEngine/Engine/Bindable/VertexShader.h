#pragma once
#include "Bindable.h"

#include <string>

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) override;
	ID3DBlob* GetBytecode() const;

protected:
	ComPtr<ID3DBlob> pBytecodeBlob;
	ComPtr<ID3D11VertexShader> pVertexShader;
};