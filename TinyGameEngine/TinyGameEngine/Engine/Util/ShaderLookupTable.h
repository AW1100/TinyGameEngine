#pragma once
#include <unordered_map>
#include <string>

enum class MeshType
{
	StylishCharacterA,
	StylishObjectA,
	RealisticCharacterA,
	RealisticObjectA
};

enum VertexType : uint32_t
{
	None = 0,
	Diffuse = 1 << 0,
	Specular = 1 << 1,
	NormalMap = 1 << 2
};

struct PairHash {
	size_t operator()(const std::pair<MeshType, int>& pair) const {
		auto hash1 = std::hash<int>{}(static_cast<int>(pair.first));
		auto hash2 = std::hash<int>{}(pair.second);
		return hash1 ^ (hash2 << 1);
	}
};

class ShaderLookupTable
{
public:
	ShaderLookupTable()
	{
		table[std::make_pair(MeshType::StylishCharacterA, Diffuse)] = "ToonPS.cso";
		table[std::make_pair(MeshType::RealisticObjectA, Diffuse)] = "BlinnPhongDPS.cso";
		table[std::make_pair(MeshType::RealisticObjectA, Diffuse | Specular)] = "BlinnPhongDSPS.cso";
		table[std::make_pair(MeshType::RealisticObjectA, Diffuse | NormalMap)] = "BlinnPhongDSPS.cso";
		table[std::make_pair(MeshType::RealisticObjectA, Diffuse | Specular | NormalMap)] = "BlinnPhongDSPS.cso";
		table[std::make_pair(MeshType::RealisticObjectA, None)] = "BlinnPhongDSPS.cso";
	} 

	const std::string& GetShaderfilename(MeshType mt, unsigned int vt) const
	{
		auto target = table.find(std::make_pair(mt, vt));
		assert(target != table.end());
		return target->second;
	}

private:
	std::unordered_map<std::pair<MeshType, int>, std::string, PairHash> table;
};