struct GSOutput
{
	float4 pos : SV_POSITION;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float3 normal : Normal;
};

[maxvertexcount(3)]
void main(
	triangle VSOutput input[3], 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
        element.pos = input[i].pos + float4(input[i].normal * 0.01f, 0.0f);
		output.Append(element);
	}
}