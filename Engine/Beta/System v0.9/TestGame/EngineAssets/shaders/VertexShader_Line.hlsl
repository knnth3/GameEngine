cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 color;
	float3 cameraPos;
	float padding;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


VSOutput main( float4 pos : POSITION )
{
	VSOutput output;
	// Change the position vector to be 4 units for proper matrix calculations.
	pos.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(pos, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.color = color;

	return output;
}