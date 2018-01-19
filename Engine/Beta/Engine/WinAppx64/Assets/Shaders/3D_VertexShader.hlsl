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
	float3 worldPos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
    float4 color : COLOR;
	float3 diffuse : COLOR01;
	float metallic : COLOR02;
	float roughness : COLOR03;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
    float4 camera : CAMERA;
    bool hasUV : FLAGS;
};

VSOutput main(Vertex input)
{
	VSOutput output;
	output.roughness = input.normal.w;
	input.normal.w = 0.0f;

	output.position = mul(input.position, instances[input.instanceID].worldMatrix);
	output.worldPos = output.position.xyz;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.normal = mul(input.normal, instances[input.instanceID].worldMatrix).xyz;
	output.normal = normalize(output.normal);

	output.tangent = mul(input.tangent.xyz, (float3x3)instances[input.instanceID].worldMatrix);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal.xyz, (float3x3)instances[input.instanceID].worldMatrix);
	output.binormal = normalize(output.binormal);

	output.metallic = instances[input.instanceID].diffuse.w;
	output.uv = input.uv;
	output.diffuse = instances[input.instanceID].diffuse.xyz;

    output.color = input.color;
    output.hasUV = flags.a;

    output.camera = camera;

	return output;
}