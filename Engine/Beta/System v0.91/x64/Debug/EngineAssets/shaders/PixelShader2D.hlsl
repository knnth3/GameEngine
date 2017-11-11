#include "VertexShader2D.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;



float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = ObjTexture.Sample(ObjSamplerState, input.uv);
	return input.color * texColor;
}