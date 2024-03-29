cbuffer LightCBuf : register(b0)
{
    float3 lightPos;
};

cbuffer ModelMatrix : register(b1)
{
    matrix model;
}

Texture2DArray texArray : register(t0);
SamplerState texSampler : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
    float3 tex : TexCoord0; // The third component (tex.z) will specify the texture slice index.
    float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_Target
{
    float4 textureColor = texArray.Sample(texSampler, input.tex);
    float dist = distance(input.worldPos, lightPos);
    // Normalize the incoming normal vector.
    float3 norm;
    if (abs(input.tex.z - 4.0f) < 0.01)
    {
        norm = normalize(texArray.Sample(texSampler, float3(input.tex.x, input.tex.y, 6.0f)));
    }
    else
    {
        norm = normalize(input.normal);
    }
    
    const float3 vToL = lightPos - input.worldPos;
    // Calculate the diffuse lighting factor.
    float diff = max(dot(norm, vToL), 0.0f);

    // Quantize the diffuse factor to create the toon effect.
    // Adjust the number of levels to control the toon effect's granularity.
    const int levels = 3;
    diff = floor(diff * levels) / (levels - 1);

    // Define some arbitrary colors for the toon effect.
    float3 lightColor = float3(0.6, 0.6, 0.6);
    float3 darkColor = float3(0.0, 0.0, 0.0);

    // Lerp between a light and a dark color based on the quantized diffuse factor.
    float3 color = clamp(lerp(darkColor, lightColor, diff), 0.0f, 1.0f);
    
    float att = 1.0f / dist;

    // Return the final color.
    return float4(0.7 * textureColor.xyz + 0.3 * att * color, 1.0f);
}