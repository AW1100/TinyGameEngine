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
static const float attLin = 0.05f;
static const float attQuad = 0.1f;

Texture2DArray texArray : register(t0);
SamplerState texSampler : register(s0);

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
    
    float distance = length(lightPos.xyz - input.worldPos);
    float attenuation = 1.0f / (attConst + attLin * distance + attQuad * distance * distance);
    
    const float3 vToL = normalize(lightPos.xyz - input.worldPos);
    const float diffuse = max(0.0f, dot(vToL, input.normal));
    
    const float3 vToC = normalize(eyePos.xyz - input.worldPos);
    
    float3 output = { 0.0f, 0.0f, 0.0f };
    if (abs(diffuse - 0.0f) < threshold)
    {
        output += ambient;
    }
    else
    {
        const float3 halfVector = normalize(vToL + vToC);
        const float specular = pow(max(0.0f, dot(halfVector, input.normal)), alpha);
        const float4 specularColor = texArray.Sample(texSampler, float3(input.tex.x, input.tex.y, input.tex.z + 1.0f));
        output = ambient + attenuation * (diffuse * textureColor.xyz + diffuse * lightColor.xyz * specularColor.xyz * specular);
    }

    return float4(output, textureColor.a);
}