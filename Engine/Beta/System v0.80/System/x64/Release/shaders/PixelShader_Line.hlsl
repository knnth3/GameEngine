#include "VertexShader_Line.hlsl"

float4 main(VSOutput input) : SV_TARGET
{
	return input.color;
}