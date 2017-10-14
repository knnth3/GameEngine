#include "VertexShader2D.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;



float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = ObjTexture.Sample(ObjSamplerState, input.uv);
	float4 output =  float4(1.0f, 1.0f, 1.0f, 1.0f);
	return output;
}