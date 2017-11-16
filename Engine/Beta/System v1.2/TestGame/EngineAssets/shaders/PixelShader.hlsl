#include "VertexShader.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;


float4 main(VSOutput input) : SV_TARGET
{

	return input.color;
}