#pragma once

struct Vertex
{
    struct
    {
        float x;
        float y;
        float z;
    }pos;
    struct
    {
        float u;
        float v;
    }tex;
    struct
    {
        float x;
        float y;
        float z;
    }normal;

    bool operator==(const Vertex& rhs) const
    {
        return pos.x == rhs.pos.x && pos.y == rhs.pos.y && pos.z == rhs.pos.z;
    }
};

struct UV
{
    struct
    {
        float u;
        float v;
    }uv;
};