cbuffer ConstBuffer
{
	float4x4 WVP;
};

struct VSOutput
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VSOutput main( float4 pos : POSITION, float4 color : COLOR)
{
	VSOutput output;
	output.Pos = mul(pos, WVP);
	output.Color = color;
	return output;
}