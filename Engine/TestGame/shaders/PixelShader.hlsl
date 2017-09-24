#include "VertexShader.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
	return input.Color;
}