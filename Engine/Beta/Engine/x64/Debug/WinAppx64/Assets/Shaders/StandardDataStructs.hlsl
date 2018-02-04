
#define MAX_JOINTS_PER_VERTEX 3
struct Instance
{
    float4x4 worldMatrix;
    float4 diffuse;
    float4 textureBounds;
};

struct Vertex
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 color : COLOR;
    uint3 jointIDs : JOINT_ID;
    float3 jointWeights : JOINT_WEIGHT;
    uint instanceID : SV_InstanceID;
};