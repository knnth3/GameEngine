#include "GeometryShader2D.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;



float4 main(GSOutput input) : SV_TARGET
{
	float4 texColor = ObjTexture.Sample(ObjSamplerState, input.uv);
	float avg = (texColor.r + texColor.g + texColor.b) / 3.0f;

	texColor.r = avg;
	texColor.g = avg;
	texColor.b = avg;
	float4 output = saturate(texColor * input.color);
	return output;
}