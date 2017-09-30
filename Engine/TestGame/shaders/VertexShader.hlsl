cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	//float4x4 WVP;
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 texCoord  : TEXCOORD;
	bool useColor : USECOLOR;
};

VSOutput main( VSInput input )
{

	VSOutput output;
	input.pos.w = 1.0f;
	output.pos = mul(input.pos, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);
	output.texCoord = input.uv;
	output.useColor = false;

	return output;
}