
struct Vertex
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float4 normal : NORMAL;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
	float4 color : COLOR;
	uint instanceID : SV_InstanceID;
};