#include "StandardDataStructs.hlsl"


#define MAX_INSTANCES 500

cbuffer ConstBuffer
{
	bool4 flags;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
    float4 camera;
	Instance instances[MAX_INSTANCES];
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VSOutput main(Vertex input)
{
	VSOutput output;
    output.position.z = 1.0f;
	output.position = mul(input.position, instances[input.instanceID].worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    output.color = float4(input.color, 1.0f);

	return output;
}