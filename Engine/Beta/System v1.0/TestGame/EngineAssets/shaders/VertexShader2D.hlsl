cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 color;
	float3 cameraPos;
	float padding;
};

struct VSInput
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv  : TEXCOORD;
	float4 color : COLOR01;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position.w = 1.0f;
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.color = color;
	output.uv = input.uv;
	return output;
}