#include "ShadowLevel.hlsli"
#include "SpecularColor.hlsli"
#include "LightingConstants.hlsli"

cbuffer LightCBuf
{
    float4 lightPos;
    float4 lightColor;
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
    float3 tan : TANGENT;
    float3 bitan : BITANGENT;
};

float4 main(PixelInputType input) : SV_Target
{
    const float4 textureColor = texArray.Sample(texSampler, input.tex);
    clip(textureColor.a < 0.1f ? -1 : 1);
    const float threshold = 0.0001f;
    if (dot(eyePos.xyz - input.worldPos, input.normal) < 0.0f)
    {
        input.normal = -input.normal;
    }
    
    const float3x3 tanToWorld = float3x3(normalize(input.tan), normalize(input.bitan), normalize(input.normal));
    const float3 norm = texArray.Sample(texSampler, float3(input.tex.x, input.tex.y, input.tex.z + 2.0f)).xyz;
    float3 n = normalize(mul(float3(norm.x * 2.0f - 1.0f, -norm.y * 2.0f + 1.0f, norm.z), tanToWorld));
    
    float distance = length(lightPos.xyz - input.worldPos);
    float attenuation = 1.0f / (attConst + attLin * distance + attQuad * distance * distance);
    
    const float3 vToL = normalize(lightPos.xyz - input.worldPos);
    const float diffuse = max(0.0f, dot(vToL, n));
    
    float3 lightToPixel = input.worldPos - lightPos.xyz;
    
    const float3 vToC = normalize(eyePos.xyz - input.worldPos);
    
    float3 output = ambient;
    if (abs(diffuse - 0.0f) > threshold)
    {
        const float3 specularColor = CalculateSpecularColor(vToL, vToC, texArray, texSampler, input.tex, n, alpha);
        float shadowLevel = SampleShadow(shadowMap, shadowSampler, lightToPixel, distance);
        output = output + attenuation * (diffuse * textureColor.xyz + diffuse * lightColor.xyz * specularColor) * shadowLevel;
    }

    return float4(output, textureColor.a);
}