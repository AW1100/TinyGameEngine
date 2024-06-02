#pragma once
#include "Bindable.h"

#include <string>

class GeometryShader : public Bindable
{
public:
	GeometryShader(Graphics& gfx, const std::wstring& path);
	void Bind(Graphics& gfx) override;

private:
	ComPtr<ID3D11GeometryShader> pGeometryShader;
};