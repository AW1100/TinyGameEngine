cbuffer LightCBuf
{
    float4 lightPos;
    float4 lightColor;
};

cbuffer cameraCBuf
{
    float4 eyePos;
};

static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float alpha = 10.0f;

static const float attConst = 0.7f;
static const float attLin = 0.3f;
static const float attQuad = 0.2f;

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
#define PCF_RANGE 2
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
    float currentDepth = length(lightToPixel);
    
    float shadowLevel = 0.0f;
    float texelSize = 4.0f / 1024.0f;
    [unroll]
    for (int x = -PCF_RANGE; x <= PCF_RANGE; ++x)
    {
        [unroll]
        for (int y = -PCF_RANGE; y <= PCF_RANGE; ++y)
        {
            float3 offset = float3(x * texelSize, y * texelSize, 0.0f);
            shadowLevel += shadowMap.SampleCmpLevelZero(shadowSampler, lightToPixel+offset, distance / 50.0f - 0.0005f).r;
        }

    }
    shadowLevel /= ((PCF_RANGE * 2 + 1) * (PCF_RANGE * 2 + 1));
    shadowLevel = shadowLevel * 0.7f + 0.3f;
    
    const float3 vToC = normalize(eyePos.xyz - input.worldPos);
    
    float3 output = ambient;
    if (abs(diffuse - 0.0f) > threshold)
    {
        const float3 halfVector = normalize(vToL + vToC);
        const float specular = pow(max(0.0f, dot(halfVector, n)), alpha);
        const float4 specularColor = texArray.Sample(texSampler, float3(input.tex.x, input.tex.y, input.tex.z + 1.0f));
        output = output + attenuation * (diffuse * textureColor.xyz + diffuse * lightColor.xyz * specularColor.xyz * specular) * shadowLevel;
    }

    return float4(output, textureColor.a);
    //return float4(norm, 1.0f);
    //return float4((n * 0.5f) + 0.5f, 1.0f);
}