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

static const float attConst = 0.9f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

Texture2DArray texArray : register(t0);
SamplerState texSampler : register(s0);

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
    const float threshold = 0.0001f;
    
    const float3x3 tanToWorld = float3x3(normalize(input.tan), normalize(input.bitan), normalize(input.normal));
    const float3 norm = texArray.Sample(texSampler, float3(input.tex.x, input.tex.y, input.tex.z + 2.0f)).xyz;
    const float3 n = normalize(mul(float3(norm.x * 2.0f - 1.0f, -norm.y * 2.0f + 1.0f, norm.z), tanToWorld));
    
    float distance = length(lightPos.xyz - input.worldPos);
    float attenuation = 1.0f / (attConst + attLin * distance + attQuad * distance * distance);
    
    const float3 vToL = normalize(lightPos.xyz - input.worldPos);
    const float diffuse = max(0.0f, dot(vToL, n));
    
    const float3 vToC = normalize(eyePos.xyz - input.worldPos);
    
    float3 output = { 0.0f, 0.0f, 0.0f };
    if (abs(diffuse - 0.0f) < threshold)
    {
        output += ambient;
    }
    else
    {
        const float3 halfVector = normalize(vToL + vToC);
        const float specular = pow(max(0.0f, dot(halfVector, n)), alpha);
        const float4 specularColor = texArray.Sample(texSampler, float3(input.tex.x, input.tex.y, input.tex.z + 1.0f));
        output = ambient + attenuation * (diffuse * textureColor.xyz + diffuse * lightColor.xyz * specularColor.xyz * specular);
    }

    //return float4(output, 1.0f);
    return float4((n * 0.5f) + 0.5f, 1.0f);
}