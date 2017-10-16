#include "VertexShader2D.hlsl"

cbuffer ConstBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ConstBuffer
{
	float4 color;
	float length;
	float width;
	float2 padding;
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
		float3(-length/2.0f, width / 2.0f, 1.0f),
		float3(length, 0.0f, 1.0f), 
		float3(0.0f, -width, 1.0f),

		float3(length, 0.0f, 1.0f),
		float3(length, -width, 1.0f),
		float3(0.0f, -width, 1.0f)
	};
	for (uint i = 0; i < 6; i++)
	{

		GSOutput element;
		float3 pos = input[0].position.xyz + positions[i];
		element.position = mul(float4( pos, 1.0f ), worldMatrix);
		element.position = mul(element.position, viewMatrix);
		element.position = mul(element.position, projectionMatrix);
		element.uv = input[0].uv;
		element.color = color;
		output.Append(element);
	}
}