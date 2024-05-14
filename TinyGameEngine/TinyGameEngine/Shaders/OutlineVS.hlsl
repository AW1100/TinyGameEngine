cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

float4 main(float3 pos : POSITION, float3 n : NORMAL) : SV_POSITION
{
    return mul(float4(pos + n * 0.01, 1.0f), modelViewProj);
}