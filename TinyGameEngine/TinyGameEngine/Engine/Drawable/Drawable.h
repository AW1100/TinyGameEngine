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
	void AddBind(std::unique_ptr<class Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ib);

private:
	virtual std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;

protected:
	IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};