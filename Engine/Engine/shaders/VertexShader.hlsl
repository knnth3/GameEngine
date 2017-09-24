cbuffer ConstBuffer
{
	float4x4 WVP;
};

struct VSOutput
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 TexCoord  : TEXCOORD;
	bool useColor : USECOLOR;
};

VSOutput main( float4 pos : POSITION, float4 color : COLOR, float2 inTexCoord : TEXCOORD)
{
	VSOutput output;
	output.Pos = mul(pos, WVP);
	output.Color = color;
	output.TexCoord = inTexCoord;
	if (color.r == 1.0f && color.b == 1.0f && color.b == 1.0f)
	{
		output.useColor = false;
	}
	else
		output.useColor = true;

	return output;
}