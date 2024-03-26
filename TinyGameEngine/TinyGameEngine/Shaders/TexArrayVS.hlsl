cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float4 pos : SV_Position;
    float3 tex : TexCoord;
    float3 normal : Normal;
};

VSOut main(float3 pos : Position, float3 tex : TexCoord, float3 normal : Normal)
{
    VSOut vso;
    vso.tex = tex;
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}