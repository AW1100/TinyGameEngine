cbuffer LightCBuf
{
    float4 lightPos;
    float4 lightColor;
};

struct VSInput
{
    float4 worldPos : WORLDPOS;
    float4 pos : SV_Position;
};

struct PSOutput
{
    float4 color : SV_Target; // Color output
    float depth : SV_Depth; // Depth output
};

PSOutput main(VSInput input)
{
    PSOutput output;
    float lightDistance = length(input.worldPos.xyz - lightPos.xyz);
    lightDistance = lightDistance / 50.0f;
    output.color = float4(lightDistance, lightDistance, lightDistance, 1.0f);
    output.depth = lightDistance;
    return output;
}