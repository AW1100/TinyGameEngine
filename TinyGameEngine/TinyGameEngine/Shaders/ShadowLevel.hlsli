#define PCF_RANGE 2
#define LOWER_BOUND 0.3f
#define BIAS 0.0005f
#define NUM_OF_TEXELS 4.0f
#define SHADOWMAP_SIZE 1024.0f
#define FAR 100.0f
float SampleShadow(TextureCube shadowMap, SamplerComparisonState shadowSampler, float3 lightToPixel, float distance)
{
    float shadowLevel = 0.0f;
    float texelSize = NUM_OF_TEXELS / SHADOWMAP_SIZE;
    [unroll]
    for (int x = -PCF_RANGE; x <= PCF_RANGE; ++x)
    {
        [unroll]
        for (int y = -PCF_RANGE; y <= PCF_RANGE; ++y)
        {
            float3 offset = float3(x * texelSize, y * texelSize, 0.0f);
            shadowLevel += shadowMap.SampleCmpLevelZero(shadowSampler, lightToPixel + offset, distance / FAR - BIAS).r;
        }

    }
    shadowLevel /= ((PCF_RANGE * 2 + 1) * (PCF_RANGE * 2 + 1));
    shadowLevel = shadowLevel * (1.0f - LOWER_BOUND) + LOWER_BOUND;
    
    return shadowLevel;
}