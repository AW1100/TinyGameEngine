const float3 cbuf[] =
{
    {
        { 1.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 1.0f },
    }
};

float4 main(uint tid : SV_PRIMITIVEID) : SV_TARGET
{
    return float4(cbuf[tid / 2], 1.0f);
}