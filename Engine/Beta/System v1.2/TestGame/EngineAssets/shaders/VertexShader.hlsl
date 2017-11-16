cbuffer ConstBuffer
{
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VSInput
{
	//Vertex Info
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;

	//Instance Info
	float4 color : COLOR;
	float4x4 worldMatrix : WORLDMAT;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VSOutput main( VSInput input )
{
	VSOutput output;
	// Change the position vector to be 4 units for proper matrix calculations.
	float4 inpos = float4(input.position, 1.0f);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(inpos, input.worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.color = input.color;

	return output;

}