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
	float3 normal : NORMAL;
	float4 color : COLOR;
};

//------------------------Vertex Shader-----------------------------------
VSOutput vsMain(VSInput input)
{
	VSOutput output;

	// Change the position vector to be 4 units for proper matrix calculations.
	float4 inpos = float4(input.position, 1.0f);

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(inpos, instances[input.instanceID].worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.normal = input.normal;
	// Calculate the position of the vertex against the world, view, and projection matrices.
	//output.normal = mul(normal, instances[input.instanceID].worldMatrix);
	//output.normal = mul(output.normal, viewMatrix);
	//output.normal = mul(output.normal, projectionMatrix);

	output.color = instances[input.instanceID].color;

	return output;
}

//------------------------Vertex Shader-----------------------------------



//------------------------Pixel Shader------------------------------------
Texture2D ObjTexture;
SamplerState ObjSamplerState;


float4 psMain(VSOutput input) : SV_TARGET
{
	static const float PI = 3.14159265f;
	float4 ambient = float4(0.1f, 0.1f, 0.1f, 1.0f);
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	float3 lightDirection = float3(0.0f, 1.0f, 0.0f);
	float3 NDotL = dot(input.normal, lightDirection);
	float3 diffuse = (lightColor * NDotL) / PI;
	float4 finalColor = input.color * float4(diffuse, 1.0f);
	//return finalColor + ambient;
	return float4(input.normal, 1.0f);
}

//------------------------Pixel Shader------------------------------------
