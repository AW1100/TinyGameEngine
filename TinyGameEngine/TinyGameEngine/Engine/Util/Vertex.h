#pragma once
#include <DirectXMath.h>

struct Vertex
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT3 texCoord;
    DirectX::XMFLOAT3 normal;

    bool operator==(const Vertex& rhs) const
    {
        return pos.x == rhs.pos.x && pos.y == rhs.pos.y && pos.z == rhs.pos.z;
    }
};
