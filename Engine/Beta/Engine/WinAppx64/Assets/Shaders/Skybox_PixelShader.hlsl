Texture2D ObjTexture[5];
SamplerState ObjSamplerState;

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 diffuse : COLOR01;
	float metallic : COLOR02;
	float roughness : COLOR03;
    float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

float4 main(VSOutput input) : SV_TARGET
{
	float4 tex = ObjTexture[0].Sample(ObjSamplerState, input.uv);
	//tex.w = 1.0f;
	float3 lightPos = float3(0.0f, 500.0f, 0.0f);
	float3 lightVec = normalize(lightPos - input.worldPos);
	float brightness = saturate(dot(input.normal, lightVec));
	float4 ambient = float4(0.1f, 0.1f, 0.1f, 0.0f);
	float4 color = float4(input.diffuse * brightness, 1.0f) + ambient;

	return tex * color;
}