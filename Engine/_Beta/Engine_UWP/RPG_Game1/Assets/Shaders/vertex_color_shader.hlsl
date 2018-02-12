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
	float3 color : COLOR;
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
	float3 color : COLOR;
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

	output.color = input.color;

	return output;
}

//------------------------Vertex Shader-----------------------------------



//------------------------Pixel Shader------------------------------------

float4 psMain(VSOutput input) : SV_TARGET
{
	float3 lightVec = float3(0.0f, 0.7f, 0.0f);
	float brightness = saturate(dot(input.normal, lightVec));
	float4 ambient = float4(0.2f, 0.2f, 0.2f, 0.0f);
	float4 color = float4(input.diffuse * brightness, 1.0f);
	color = saturate(color + ambient);

	return (float4(input.color, 1.0f) * color);
}

//------------------------Pixel Shader------------------------------------
