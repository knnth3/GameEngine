Texture2D ObjTexture[5];
SamplerState ObjSamplerState;

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 worldPos : POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
    float3 diffuse : COLOR01;
    float metallic : COLOR02;
    float roughness : COLOR03;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 camera : CAMERA;
    bool hasUV : FLAGS;
};

float4 main(VSOutput input) : SV_TARGET
{
    float3 lightVec = float3(0.0f, 1.0f, 0.0f);
    float brightness = saturate(dot(lightVec, input.normal.xyz));
    float4 ambient = float4(0.05f, 0.05f, 0.05f, 0.0f);
    float4 color = float4(input.diffuse * brightness, 1.0f);

    color = color * input.color;

    if(input.hasUV)
    {
        float4 tex = ObjTexture[0].Sample(ObjSamplerState, input.uv);
        color = color * tex;

    }

    color = color + ambient;
    return color;

}