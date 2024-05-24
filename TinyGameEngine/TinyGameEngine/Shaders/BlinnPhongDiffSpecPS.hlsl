#include "ShadowLevel.hlsli"
#include "SpecularColor.hlsli"
#include "LightingConstants.hlsli"

cbuffer LightCBuf
{
    float4 lightPos;
    float4 lightColor;
    float4 attConsts;
};

cbuffer cameraCBuf
{
    float4 eyePos;
};

Texture2DArray texArray : register(t0);
SamplerState texSampler : register(s0);

TextureCube shadowMap : register(t1);
SamplerComparisonState shadowSampler : register(s1);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float3 tex : TexCoord0;
    float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_Target
{
    const float4 textureColor = texArray.Sample(texSampler, input.tex);
    const float threshold = 0.0001f;
    if (dot(eyePos.xyz - input.worldPos, input.normal) < 0.0f)
    {
        input.normal = -input.normal;
    }
    
    float distance = length(lightPos.xyz - input.worldPos);
    float attenuation = 1.0f / (attConsts.x + attConsts.y * distance + attConsts.z * distance * distance);
    
    const float3 vToL = normalize(lightPos.xyz - input.worldPos);
    const float diffuse = max(0.0f, dot(vToL, input.normal));
    
    float3 lightToPixel = input.worldPos - lightPos.xyz;
    
    const float3 vToC = normalize(eyePos.xyz - input.worldPos);
    
    float3 output = { 0.0f, 0.0f, 0.0f };
    if (abs(diffuse - 0.0f) < threshold)
    {
        output += ambient;
    }
    else
    {
        const float3 specularColor = CalculateSpecularColor(vToL, vToC, texArray, texSampler, input.tex, input.normal, attConsts.w);
        float shadowLevel = SampleShadow(shadowMap, shadowSampler, lightToPixel, distance);
        output = ambient + attenuation * (diffuse * textureColor.xyz + diffuse * lightColor.xyz * specularColor) * shadowLevel;
    }

    return float4(output, textureColor.a);
}