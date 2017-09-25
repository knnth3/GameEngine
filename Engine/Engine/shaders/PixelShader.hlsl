#include "VertexShader.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;

cbuffer TransparentBuffer
{
	float4 blendAmount;
};

float4 main(VSOutput input) : SV_TARGET
{
	float4 color;
	color = ObjTexture.Sample(ObjSamplerState, input.texCoord);
	color.a = blendAmount.a;
	return color;
}