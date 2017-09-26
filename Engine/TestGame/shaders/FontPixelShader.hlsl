#include "FontVertexShader.hlsl"
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
	if (color.a > blendAmount.a)
	{
		color = blendAmount;
	}
	return color;
}