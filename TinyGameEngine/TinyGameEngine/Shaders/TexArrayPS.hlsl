// Define a Texture2DArray with a register that matches the one used in the application.
Texture2DArray texArray : register(t0);
SamplerState texSampler : register(s0);

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 tex : TexCoord; // The third component (tex.z) will specify the texture slice index.
};

float4 main(PixelInputType input) : SV_TARGET
{
    // Sample the texture array.
    // The third component of the tex coord (input.tex.z) specifies which texture to use.
    float4 textureColor = texArray.Sample(texSampler, input.tex);

    // You could also do additional calculations here if needed.

    return textureColor;
}
