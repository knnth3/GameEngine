#include "GeometryShader2D.hlsl"
Texture2D ObjTexture;
SamplerState ObjSamplerState;



float4 main(GSOutput input) : SV_TARGET
{
	float4 texColor = ObjTexture.Sample(ObjSamplerState, input.uv);
	float4 output =  input.color;
	return output;
}