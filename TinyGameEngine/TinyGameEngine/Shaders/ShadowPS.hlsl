float4 main(float3 viewPos : Position) : SV_TARGET
{
    float ret = length(viewPos) / 100.0f;
    return float4(ret, ret, ret, 1.0f);
}