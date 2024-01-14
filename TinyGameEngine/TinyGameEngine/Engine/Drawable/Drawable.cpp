#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<class Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> ib)
{
	pIndexBuffer = ib.get();
	binds.push_back(std::move(ib));
}
