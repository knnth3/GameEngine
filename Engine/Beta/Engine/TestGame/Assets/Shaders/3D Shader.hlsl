#define MAX_INSTANCES 500

struct Instance
{
	float4x4 worldMatrix;
	float4 color;
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
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	uint instanceID : SV_InstanceID;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

//------------------------Vertex Shader-----------------------------------
VSOutput vsMain(VSInput input)
{
	VSOutput output;

	float4 position = float4(input.position, 1.0f);
	position = mul(position, instances[input.instanceID].worldMatrix);
	output.pos = position.xyz;
	position = mul(position, viewMatrix);
	position = mul(position, projectionMatrix);
	output.position = position;

	output.normal = mul(input.normal, (float3x3)instances[input.instanceID].worldMatrix);
	output.normal = normalize(output.normal);
	output.uv = input.uv;
	output.color = instances[input.instanceID].color;

	return output;
}

//------------------------Vertex Shader-----------------------------------



//------------------------Pixel Shader------------------------------------
Texture2D ObjTexture;
SamplerState ObjSamplerState;


float4 psMain(VSOutput input) : SV_TARGET
{
	float4 tex = ObjTexture.Sample(ObjSamplerState, input.uv);
	float3 lightPos = float3(0.0f, 500.0f, 0.0f);
	float3 lightVec = normalize(lightPos - input.pos);
	float brightness = saturate(dot(input.normal, lightVec));
	float4 color = float4(input.color.xyz * brightness, input.color.w);

	return tex * color;
}

//------------------------Pixel Shader------------------------------------
