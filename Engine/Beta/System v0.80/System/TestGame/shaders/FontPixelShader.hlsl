#include "FontVertexShader.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(VSOutput input) : SV_TARGET
{
	float4 color;
	color = ObjTexture.Sample(ObjSamplerState, input.texCoord);
	if (color.a >= input.color.a)
	{
		color = input.color;
	}
	else
	{
		color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	return color;
}