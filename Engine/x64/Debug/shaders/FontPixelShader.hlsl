#include "FontVertexShader.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;

cbuffer PF_PixelBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float3 sunDirection;
	float specularPower;
};


float4 main(VSOutput input) : SV_TARGET
{
	float4 color;
	color = ObjTexture.Sample(ObjSamplerState, input.texCoord);
	if (color.a >= specularColor.a)
	{
		color = specularColor;
	}
	else
	{
		color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	return color;
}