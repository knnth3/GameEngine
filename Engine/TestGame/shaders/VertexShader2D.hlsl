




struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv  : TEXCOORD;
};

VSOutput main(float4 position : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{

	VSOutput output;
	output.position = position;
	output.uv = inTexCoord;

	return output;
}