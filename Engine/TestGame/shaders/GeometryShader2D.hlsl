#include "VertexShader2D.hlsl"

cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

[maxvertexcount(6)]
void main(point VSOutput input[1], inout TriangleStream<GSOutput> output)
{

	float3 positions[6] = 
	{ 
		float3(0.0f, 0.0f, 1.0f), 
		float3(input[0].length, 0.0f, 1.0f), 
		float3(0.0f, -input[0].width, 1.0f),

		float3(input[0].length, 0.0f, 1.0f),
		float3(input[0].length, -input[0].width, 1.0f),
		float3(0.0f, -input[0].width, 1.0f)
	};
	for (uint i = 0; i < 6; i++)
	{

		GSOutput element;
		float3 pos = input[0].position + positions[i];
		element.position = mul(float4( pos, 1.0f ), worldMatrix);
		element.position = mul(element.position, viewMatrix);
		element.position = mul(element.position, projectionMatrix);
		element.uv = input[0].uv;
		element.color = input[0].color;
		output.Append(element);
	}
}