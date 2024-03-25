cbuffer CBuf
{
    matrix transform;
};

struct VSOut
{
    float4 pos : SV_Position;
    float3 tex : TexCoord;
};

VSOut main(float3 pos : Position, float3 tex : TexCoord, float3 normal : Normal)
{
    VSOut vso;
    vso.tex = tex;
    vso.pos = mul(float4(pos, 1.0f), transform);
    return vso;
}