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
	float3 normal : NORMAL;
	float3 viewDirection : VIEW;
	float4 color : COLOR;
};

VSOutput main( VSInput input )
{
	VSOutput output;
	float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.uv = input.uv;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);
	//The viewing direction is calculated here in the vertex shader.We calculate the world position of the vertex and subtract that from the camera position to determine where we are viewing the scene from.The final value is normalized and sent into the pixel shader.

	// Calculate the position of the vertex in the world.
	worldPosition = mul(input.position, worldMatrix);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection =  -(cameraPos.xyz - worldPosition.xyz);

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	output.color = color;

	return output;

}