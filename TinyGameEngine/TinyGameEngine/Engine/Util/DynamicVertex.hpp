#pragma once
#include <vector>
#include <algorithm>

enum ElementType
{
	Position3D,
	Texture2D,
	Normal,
	Tangent,
	Bitangent
};

class VertexLayout
{
public:

	class Element
	{
	public:
		Element(ElementType et)
			:type(et)
		{
			switch (type)
			{
			case Position3D:
				sizeInByte = 12;
				semantic = "Position";
				break;
			case Texture2D:
				sizeInByte = 12;
				semantic = "TexCoord";
				break;
			case Normal:
				sizeInByte = 12;
				semantic = "Normal";
				break;
			case Tangent:
				sizeInByte = 12;
				semantic = "Tangent";
				break;
			case Bitangent:
				sizeInByte = 12;
				semantic = "Bitangent";
				break;
			default:
				sizeInByte = 0;
				semantic = "Unknown";
				break;
			}
		}

		ElementType type;
		unsigned int sizeInByte;
		const char* semantic = nullptr;
	};

	VertexLayout()
		:count(0), size(0)
	{}

	void Append(Element et)
	{
		count++;
		size += et.sizeInByte;
		elements.push_back(et);
	}

	std::vector<Element> elements;
	unsigned int count;
	unsigned int size;
};

class DynamicVertex
{
public:
	DynamicVertex() {}

	DynamicVertex(VertexLayout vl)
		:layout(vl)
	{

	}

	UINT GetStride() const
	{
		return layout.size;
	}

	template <typename T>
	void Store(const T& value)
	{
		const char* start = reinterpret_cast<const char*>(&value);
		contents.insert(contents.end(), start, start + sizeof(T));
	}

	template <typename T>
	T Retrieve(size_t index, const char* name)
	{
		/*unsigned int offset = 0;
		for (int i = 0; i < layout.elements.size(); i++)
		{
			if (strcmp(layout.elements[i].semantic) == 0)
			{
				break;
			}
			offset += layout.elements[i].offset;
		}*/



	}
	
	inline VertexLayout& GetLayout() { return layout; }
	inline std::vector<char>& GetContents() { return contents; }

private:
	VertexLayout layout;
	std::vector<char> contents;
};