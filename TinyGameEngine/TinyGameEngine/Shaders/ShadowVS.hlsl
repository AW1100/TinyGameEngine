cbuffer LightMV
{
    matrix mv;
};

cbuffer LightMVP
{
    matrix mvp;
};

struct Output
{
    float3 viewPos : Position;
    float4 pos : SV_Position;
};

Output main(float3 pos : POSITION)
{
    //float4 position = mul(float4(pos, 1.0f), mvp);
    //float3 viewPos = mul(float4(pos, 1.0f), mv).xyz;
    //position.z = length(viewPos) * position.w / 100.0f;
    Output output;
    output.pos = mul(float4(pos, 1.0f), mvp);
    output.viewPos = mul(float4(pos, 1.0f), mv).xyz;
    return output;
}