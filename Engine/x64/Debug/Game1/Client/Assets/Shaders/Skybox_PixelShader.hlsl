SamplerState ObjSamplerState;
TextureCube SkyMap;

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

float4 main(VSOutput input) : SV_TARGET
{
    return SkyMap.Sample(ObjSamplerState, input.texCoord);
}