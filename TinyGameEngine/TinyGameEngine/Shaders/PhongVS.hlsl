cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float4 pos : SV_POSITION; // Use SV_POSITION for clip space position
    float3 worldPos : WORLDPOS; // Custom semantic for world position
    float3 tex : TEXCOORD0; // Standard semantic for texture coordinates
    float3 normal : NORMAL; // Standard semantic for normals
};

VSOut main(float3 pos : POSITION, float3 tex : TEXCOORD0, float3 n : NORMAL)
{
    VSOut vso;
    vso.worldPos = (float3)mul(float4(pos, 1.0f), model);
    vso.tex = tex;
    vso.normal = mul(n, (float3x3)model); // Assumes uniform scaling
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}
