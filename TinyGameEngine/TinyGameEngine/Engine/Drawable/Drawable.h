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
	virtual ~Drawable();
	virtual DirectX::XMMATRIX GetModelMatrix() const = 0;
	void Draw(Graphics& gfx) const;
	virtual void Update(float dt) = 0;
	void AddBind(std::shared_ptr<class Bindable> bind);
	void AddIndexBuffer(std::shared_ptr<class IndexBuffer> ib);

protected:
	IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bindable>> binds;
};