cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float3 worldPos : Position;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : POSITION )
{
    VSOut output;
    output.worldPos = pos;
    output.pos = mul(float4(pos, 0.0f), modelViewProj);
    output.pos.z = output.pos.w;
	return output;
}