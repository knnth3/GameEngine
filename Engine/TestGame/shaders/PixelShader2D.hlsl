#include "VertexShader2D.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;



float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = ObjTexture.Sample(ObjSamplerState, input.uv);
	float4 output =  texColor * float4(0.1f, 0.1f, 0.1f, 0.5f);
	return output;
}