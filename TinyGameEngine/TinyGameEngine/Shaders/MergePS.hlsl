Texture2D frame;
SamplerState texSampler : register(s0);

float4 main(float4 pos : SV_Position) : SV_TARGET
{
    float dx = 1.0f / 1600.0f;
    float dy = 1.0f / 900.0f;
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    color = frame.Sample(texSampler, float2(pos.x * dx, pos.y * dy));
    if (color.x == 0.0f && color.y == 0.0f && color.z == 0.0f)
    {
        color.w = 0.0f;
    }
    else
    {
        color.w = 1.0f;
    }
    return color;
}