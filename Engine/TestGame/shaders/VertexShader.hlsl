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
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD;
	float3 normal : NORMAL;
};

VSOutput main( VSInput input )
{
	VSOutput output;
	input.pos.w = 1.0f;
	//Calculate the vertex position with the matrices
	output.pos = mul(input.pos, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);

	output.uv = input.uv;
	// Calculate the normal vector with the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}