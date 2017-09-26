cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	//float4x4 WVP;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 texCoord  : TEXCOORD;
	bool useColor : USECOLOR;
};

VSOutput main( float4 pos : POSITION, float2 inTexCoord : TEXCOORD)
{

	VSOutput output;
	pos.w = 1.0f;
	output.pos = mul(pos, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);
	output.texCoord = inTexCoord;
	output.useColor = false;

	return output;
}