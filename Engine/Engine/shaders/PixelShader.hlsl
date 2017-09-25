#include "VertexShader.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(VSOutput input) : SV_TARGET
{
	float4 result;
	float4 tex = (ObjTexture.Sample(ObjSamplerState, input.TexCoord));

	if (tex.x == 0.0f && tex.y == 0.0f && tex.z == 0.0f)
	{
		result = input.Color;
	}
	else if (!input.useColor)
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