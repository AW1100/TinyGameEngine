cbuffer LightModel
{
    matrix model;
    matrix MVP;
};

struct VSOut
{
    float4 worldPos : WORLDPOS;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION)
{
    VSOut output;
    output.worldPos = mul(float4(pos, 1.0f), model);
    output.pos = mul(float4(pos, 1.0f), MVP);
    return output;
}