struct VSOutput
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VSOutput main( float4 pos : POSITION, float4 color : COLOR)
{
	VSOutput output;
	output.Pos = pos;
	output.Color = color;
	return output;
}