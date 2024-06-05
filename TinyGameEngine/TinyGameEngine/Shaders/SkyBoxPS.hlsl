TextureCube tex : register(t0);
SamplerState splr : register(s0);

struct VSOut
{
    float3 worldPos : Position;
    float4 pos : SV_Position;
};

float4 main(VSOut input) : SV_TARGET
{
    return tex.Sample(splr, input.worldPos);
}