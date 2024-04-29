#include "Drawable.h"
#include "../Bindable/IndexBuffer.h"

Drawable::~Drawable()
{

}

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::shared_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::shared_ptr<IndexBuffer> ib)
{
	pIndexBuffer = ib.get();
	binds.push_back(std::move(ib));
}
