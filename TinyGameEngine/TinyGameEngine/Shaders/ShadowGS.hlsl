// Define input structure from the vertex shader
struct VSOutput
{
    float4 pos : SV_POSITION;
};

// Define output structure for the pixel shader
struct GSOutput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLDPOS;
    uint layer : SV_RenderTargetArrayIndex;
};


cbuffer ShadowMatrices : register(b0)
{
    matrix shadowMatrices[6];
}

[maxvertexcount(18)]
void main(triangle VSOutput input[3], inout TriangleStream<GSOutput> OutputStream)
{
    for (int face = 0; face < 6; ++face)
    {
        for (int i = 0; i < 3; ++i) // for each vertex of the triangle
        {
            GSOutput output;
            output.pos = mul(input[i].pos, shadowMatrices[face]);
            output.worldPos = input[i].pos;
            output.layer = face;
            OutputStream.Append(output);
        }
        OutputStream.RestartStrip();
    }
}
