Texture2DArray gTexture : register(t0);
SamplerState gSampler : register(s0);

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
    // Define the Sobel kernel
    float sobelKernel[3][3] =
    {
        { -1.0f, 0.0f, 1.0f },
        { -2.0f, 0.0f, 2.0f },
        { -1.0f, 0.0f, 1.0f }
    };

    // Hard-coded texel size (adjust these values according to your texture size)
    float3 texelSize = float3(9.0f / 1024.0f, 9.0f / 1024.0f, 0.0f);

    float3 color = float3(0.0f, 0.0f, 0.0f);

    // Apply the Sobel kernel
    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x)
        {
            float3 offset = float3(x, y, 0.0f) * texelSize;
            color += gTexture.Sample(gSampler, input.tex + offset).rgb * sobelKernel[y + 1][x + 1];
        }
    }

    // Calculate the intensity of the edge
    float edgeIntensity = length(color);

    // Set the outline color to red where edges are detected
    float3 outlineColor = lerp(float3(0.0f, 0.0f, 0.0f), float3(1.0f, 0.0f, 0.0f), saturate(edgeIntensity));

    // Return the final color with red outline
    return float4(outlineColor, 1.0f);
}
