#include "StandardDataStructs.hlsl"

#define MAX_INSTANCES 500
#define FLAG_SIZE 4
#define MAX_JOINTS 60
#define MAX_WEIGHTS 4

cbuffer ConstBuffer
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
    float4 camera;
    float4x4 jointTransforms[MAX_JOINTS];
	Instance instances[MAX_INSTANCES];
    uint flags[FLAG_SIZE];
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
    float3 diffuse : COLOR01;
    bool hasUV : FLAGS;
};

VSOutput main(Vertex input)
{
	VSOutput output;

    //Move position of vertex to currentPose
    float4 finalLocalPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 finalLocalNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float totalWeight = input.jointWeights[0] + input.jointWeights[1] + input.jointWeights[2] + input.jointWeights[3];
    if (totalWeight == 0.0f)
    {
        finalLocalPos = input.position;
        finalLocalNormal = float4(input.normal.xyz, 0.0f);

    }
    else
    {
        for (int x = 0; x < MAX_WEIGHTS; x++)
        {
        //Position
            float4x4 jointTransform = jointTransforms[input.jointIDs[x]];
            float4 posePosition = mul(input.position, jointTransform);
            finalLocalPos += (posePosition * input.jointWeights[x]);

        //Normal
            float4 poseNormal = mul(float4(input.normal.xyz, 0.0f), jointTransform);
            finalLocalNormal += (poseNormal * input.jointWeights[x]);

        }
    }

    output.position = mul(finalLocalPos, instances[input.instanceID].worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    output.normal = mul(finalLocalNormal, instances[input.instanceID].worldMatrix);
    //output.normal = mul(output.normal, viewMatrix);
    //output.normal = mul(output.normal, projectionMatrix);
	output.normal = normalize(output.normal);
	output.uv = input.uv;
	output.diffuse = instances[input.instanceID].diffuse.xyz;

    output.color = float4(input.color, 1.0f);

    if (flags[0] == 0)
        output.hasUV = true;
    else
        output.hasUV = false;

	return output;
}