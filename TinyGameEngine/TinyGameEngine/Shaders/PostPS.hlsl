Texture2D solid;
SamplerState texSampler : register(s0);

float4 main(float4 pos : SV_Position) : SV_TARGET
{
    float dx = 1.0f / 1600.0f;
    float dy = 1.0f / 900.0f;
    float3 color;
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            color += solid.Sample(texSampler, float2((pos.x + i) * dx, (pos.y + j) * dy));
        }
    }
    //color /= 24.0f;
    return float4(color * 100.0f, 1.0f);
}