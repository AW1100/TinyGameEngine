#pragma once
#include "../Graphics/Graphics.h"
#include "../Bindable/Bindable.h"

#include <DirectXMath.h>
#include <memory>
#include <vector>

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	~Drawable() = default;
	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	void Draw(Graphics& gfx) const;
	virtual void Update(float dt) = 0;
	void AddBind(std::unique_ptr<class Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ib);

private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};