#include "VertexShader.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;

cbuffer PF_PixelBuffer
{
	float4 blendAmount;
	float4 diffuseColor;
	float3 sunDirection;
	float padding;
};

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor;
	float3 sunDir;
	float lightIntensity;
	float4 color;
	texColor = ObjTexture.Sample(ObjSamplerState, input.uv);
	if (texColor.a > blendAmount.a)
	{
		texColor.a = blendAmount.a;
	}
	//Invert the direction
	sunDir = -sunDirection;
	//find the dot between the normal and the sun direction
	lightIntensity = saturate(dot(input.normal, sunDir));
	//multipy the colors together
	color = saturate(diffuseColor * lightIntensity);
	color.a = 1.0f;
	color *= texColor;
	//color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return color;
}