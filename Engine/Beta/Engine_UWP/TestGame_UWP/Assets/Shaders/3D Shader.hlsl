#define MAX_INSTANCES 500

struct Instance
{
	float4x4 worldMatrix;
	float4 diffuse;
	float4 textureBounds;
};

cbuffer ConstBuffer
{
	bool4 flags;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	Instance instances[MAX_INSTANCES];
};

struct VSInput
{
	//Vertex Info
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float4 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	uint instanceID : SV_InstanceID;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 diffuse : COLOR01;
	float metalic : COLOR02;
	float roughness : COLOR03;
	float3 tangent : TAMGENT;
	float3 binormal : BINORMAL;
};

//------------------------Vertex Shader-----------------------------------
VSOutput vsMain(VSInput input)
{
	VSOutput output;
	output.roughness = input.normal.w;
	input.normal.w = 0.0f;

	output.position = mul(input.position, instances[input.instanceID].worldMatrix);
	output.worldPos = output.position.xyz;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.normal = mul(input.normal, instances[input.instanceID].worldMatrix).xyz;
	output.normal = normalize(output.normal);

	output.tangent = mul(input.tangent, (float3x3)instances[input.instanceID].worldMatrix);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal, (float3x3)instances[input.instanceID].worldMatrix);
	output.binormal = normalize(output.binormal);

	output.metalic = instances[input.instanceID].diffuse.w;
	output.uv = input.uv;
	output.diffuse = instances[input.instanceID].diffuse.xyz;

	return output;
}

//------------------------Vertex Shader-----------------------------------



//------------------------Pixel Shader------------------------------------
Texture2D ObjTexture[5];
SamplerState ObjSamplerState;

struct Material
{
	float3 diffuseColor : COLOR01;
	float3 glowColor : COLOR02;
	float3 normal : NORMAL;
	float glowIntensity : COLOR03;
	float roughness : COLOR04;
	float metallic : COLOR05;
};

void CreateGBufferMaterial(VSOutput input, inout Material mat)
{
	//Calculate the brightness
	float3 lightPos = float3(0.0f, 500.0f, 0.0f);
	float3 lightVec = normalize(lightPos - input.worldPos);
	float brightness = saturate(dot(input.normal, lightVec));

	//Diffuse
	mat.diffuseColor = (ObjTexture[0].Sample(ObjSamplerState, input.uv) * float4(input.diffuse, 1.0f)).xyz;

	//Normal Map
	float4 bumpMap = ObjTexture[1].Sample(ObjSamplerState, input.uv);
	bumpMap = (bumpMap  * 2.0f) - 1.0f;

	//calculate the new normal with tangent and bitangent
	mat.normal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	mat.normal = normalize(mat.normal);

	//sample from the emessive map
	float4 emessiveColor = ObjTexture[2].Sample(ObjSamplerState, input.uv);
	mat.glowColor = emessiveColor.rgb;
	mat.glowIntensity = 1.0f;

	//fill out specular information
	mat.roughness = ObjTexture[3].Sample(ObjSamplerState, input.uv).r;
	mat.metallic = ObjTexture[4].Sample(ObjSamplerState, input.uv).r;

	mat.roughness *= 1.0f;
	mat.metallic *= 1.0f;
}

struct PS_GBUFFER_OUT
{
	float4 Color : COLOR01;
	float4 Normal : NORMAL;
	float4 GlowMap : GLOW;
};

PS_GBUFFER_OUT PackGBuffer(Material mat)
{
	PS_GBUFFER_OUT output;

	//pack the color rgb = base and a = specIntensity
	output.Color = float4(mat.diffuseColor.rgb, mat.roughness);

	//pack the normals
	output.Normal = float4(mat.normal.xyz, mat.metallic);

	//pack the glow color
	output.GlowMap = float4(mat.glowColor * mat.glowIntensity, 1.0f);

	return output;
};

float4 psMain(VSOutput input) : SV_TARGET
{
	float4 tex = ObjTexture[0].Sample(ObjSamplerState, input.uv);
	tex.w = 1.0f;
	float3 lightPos = float3(0.0f, 500.0f, 0.0f);
	float3 lightVec = normalize(lightPos - input.worldPos);
	float brightness = saturate(dot(input.normal, lightVec));
	float4 ambeint = float4(0.1f, 0.1f, 0.1f, 0.0f);
	float4 color = float4(input.diffuse * brightness, 1.0f);

	return tex;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

//------------------------Pixel Shader------------------------------------
