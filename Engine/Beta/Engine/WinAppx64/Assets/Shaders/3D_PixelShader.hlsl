Texture2D ObjTexture[5];
SamplerState ObjSamplerState;

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
    float3 diffuse : COLOR01;
    bool hasUV : FLAGS;
    bool hasColor : FLAGS02;
};

float4 main(VSOutput input) : SV_TARGET
{
    float3 lightVec = float3(0.0f, 1.0f, 0.0f);
    float brightness = saturate(dot(lightVec, input.normal.xyz));
    float4 ambient = float4(0.02f, 0.02f, 0.02f, 0.0f);
    float4 color = input.color;

    if (input.hasColor)
    {
        float heavy = 0.2f;
        float light = 0.8f;
        float weightedR = (heavy * input.diffuse.r) + (light * input.color.r);
        float weightedG = (heavy * input.diffuse.g) + (light * input.color.g);
        float weightedB = (heavy * input.diffuse.b) + (light * input.color.b);
        //float weightedAvg = (0.333f * input.color.r) + (0.5f * input.color.g) + (0.166f * input.color.b);
        color = saturate(float4(weightedR, weightedG, weightedB, 1.0f));
    }
    if(input.hasUV)
    {
        float4 tex = ObjTexture[0].Sample(ObjSamplerState, input.uv);
        color = color * tex;
    }

    color = saturate((color * brightness) + ambient);
    return color;

}