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
	float2 uv  : TEXCOORD;
	float4 color : COLOR;
};

float GetTexCoord(float num, float size, float offset)
{
	float result = num * size;
	result += offset;
	return result;
}


//------------------------Vertex Shader-----------------------------------
VSOutput vsMain(VSInput input)
{
	VSOutput output;

	// Change the position vector to be 4 units for proper matrix calculations.
	float4 inpos = float4(input.position, 1.0f);

	// Calculate the position of the vertex against the world and projection matrices.
	output.position = mul(inpos, instances[input.instanceID].worldMatrix);
	output.position = mul(output.position, projectionMatrix);

	//Set Color
	output.color = instances[input.instanceID].color;

	//Set 2D UV coords
	float4 uvBounds = instances[input.instanceID].textureBounds;
	output.uv = input.uv;
	output.uv.x = GetTexCoord(input.uv.x, uvBounds.x, uvBounds.z);
	output.uv.y = GetTexCoord(input.uv.y, uvBounds.y, uvBounds.w);
	return output;
}
//------------------------Vertex Shader-----------------------------------



//------------------------Pixel Shader------------------------------------
Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 psMain(VSOutput input) : SV_TARGET
{
	float4 texColor = ObjTexture.Sample(ObjSamplerState, input.uv);
	return input.color * texColor;
}
//------------------------Pixel Shader------------------------------------