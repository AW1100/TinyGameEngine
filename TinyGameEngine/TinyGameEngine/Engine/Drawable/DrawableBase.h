#pragma once
#include "Drawable.h"

template<typename T>
class DrawableBase : public Drawable
{
protected:
	DrawableBase() = default;
	virtual ~DrawableBase()
	{
		staticBinds.clear();
	}
	bool IsStaticInitialized() const
	{
		return staticBinds.size() > 0;
	}

	static void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		staticBinds.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf)
	{
		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}

	std::vector<std::unique_ptr<Bindable>>& GetStaticBinds()
	{
		return staticBinds;
	}

	void SetIndexBufferFromStatic()
	{
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
	}
private:
	std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const override
	{
		return staticBinds;
	}

private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<typename T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;


