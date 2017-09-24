#include "VertexShader.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(VSOutput input) : SV_TARGET
{
	float4 result;
	if (!input.useColor)
	{
		result = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
	}
	else
	{
		result = (ObjTexture.Sample(ObjSamplerState, input.TexCoord) + input.Color) * 0.5f;
		result = normalize(result);
	}
	return result;
}