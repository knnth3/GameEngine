cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 color;
	float4 uvInfo;
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

float GetTexCoord(float num, float size, float offset)
{
	float result = num * size;
	result += offset;
	return result;
}

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position.w = 1.0f;
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Set Color
	output.color = color;

	//Set 2D UV coords
	output.uv.x = GetTexCoord(input.uv.x, uvInfo.x, uvInfo.z);
	output.uv.y = GetTexCoord(input.uv.y, uvInfo.y, uvInfo.w);
	return output;
}