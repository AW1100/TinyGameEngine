cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 normal : Normal;
};

VSOutput main(float3 pos : POSITION, float3 n : NORMAL)
{
    VSOutput output;
    output.position = mul(float4(pos, 1.0f), modelViewProj);
    output.normal = normalize(mul(n, (float3x3) model));
    return output;
}