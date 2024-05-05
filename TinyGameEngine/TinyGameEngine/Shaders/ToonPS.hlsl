cbuffer LightCBuf : register(b0)
{
    float4 lightPos;
    float4 lightColor;
};

cbuffer cameraCBuf : register(b1)
{
    float4 eyePos;
};

Texture2DArray texArray : register(t0);
SamplerState texSampler : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float3 tex : TexCoord0; // The third component (tex.z) will specify the texture slice index.
    float3 normal : NORMAL;
};

static const float attConst = 0.1f;
static const float attLin = 0.2f;
static const float attQuad = 0.3f;

float4 main(PixelInputType input) : SV_Target
{
    float dist = length(lightPos.xyz - input.worldPos);
    float attenuation = 1.0f / (attConst + attLin * dist + attQuad * dist * dist);
    
    float4 textureColor = texArray.Sample(texSampler, input.tex);
    
    const float3 vToL = lightPos.xyz - input.worldPos;
    // Calculate the diffuse lighting factor.
    float diff = max(dot(input.normal, vToL), 0.0f);
    
    //if (abs(input.tex.z - 4.0f) < 0.01f)
    //{
    //    return float4(0.5 * textureColor.xyz + 0.3 * max(dot(float3(0.0f,0.0f,-1.0f), vToL), 0.0f) * textureColor.xyz, 1.0f);

    //}

    const int levels = 3;
    diff = floor(diff * levels) / (levels - 1);

    // Define some arbitrary colors for the toon effect.
    float3 lightColor = textureColor.xyz;
    float3 darkColor = float3(0.0f, 0.0f, 0.0f);

    // Lerp between a light and a dark color based on the quantized diffuse factor.
    float3 color = clamp(lerp(darkColor, lightColor, diff), 0.0f, 1.0f);

    // Return the final color.
    return float4(0.8 * attenuation * textureColor.xyz + 0.2 * attenuation * color, 1.0f);
}