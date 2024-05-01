cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float4 pos : SV_POSITION; // Use SV_POSITION for clip space position
    float4 worldPos : WORLDPOS; // Custom semantic for world position
    float3 tex : TEXCOORD0; // Standard semantic for texture coordinates
    float3 normal : NORMAL; // Standard semantic for normals
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
};

VSOut main(float3 pos : POSITION, float3 tex : TEXCOORD0, float3 n : NORMAL, float3 tan : TANGENT, float3 bitan : BITANGENT)
{
    VSOut vso;
    vso.worldPos = mul(float4(pos, 1.0f), model);
    vso.tex = tex;
    vso.normal = mul(n, (float3x3) model);
    vso.tan = mul(tan, (float3x3) model);
    vso.bitan = mul(bitan, (float3x3) model);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}
