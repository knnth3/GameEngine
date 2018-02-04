#include "StandardDataStructs.hlsl"


#define MAX_INSTANCES 500
#define FLAG_SIZE 4

cbuffer ConstBuffer
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4 camera;
    Instance instances[MAX_INSTANCES];
    uint flags[FLAG_SIZE];
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

VSOutput main(Vertex input)
{
	VSOutput output;
    output.position.z = 1.0f;
	output.position = mul(input.position, instances[input.instanceID].worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    output.texCoord = input.position.xyz;

	return output;
}