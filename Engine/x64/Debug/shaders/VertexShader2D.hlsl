
cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float3 cameraPos;
	float padding;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv  : TEXCOORD;
	//float4 color : TEXTCOLOR;
};

VSOutput main(float4 pos : POSITION, float2 inTexCoord : TEXCOORD)
{

	VSOutput output;
	output.position = mul(pos, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.uv = inTexCoord;

	return output;
}