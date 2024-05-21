cbuffer LightMV
{
    matrix mv;
};

cbuffer LightMVP
{
    matrix mvp;
};

float4 main(float3 pos : POSITION) : SV_POSITION
{
    float4 position = mul(float4(pos, 1.0f), mvp);
    float3 viewPos = mul(float4(pos, 1.0f), mv).xyz;
    position.z = length(viewPos) * position.w / 100.0f;
    return position;
}