cbuffer viewBuffer : register(b0)
{
    float4x4 view;
};

struct VS_INPUT
{
    float3 inPos : POSITION;
    //float3 inNormal : NORMAL;
    float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    //float3 outNormal : NORMAL;
    float2 outTexCoord : TEXCOORD;
    //bool normalMapEnabled : NORMAL_MAP_ENABLED;
};

VS_OUTPUT VS_main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.outPos = mul(float4(input.inPos, 1.0f), view);
    //output.outNormal = input.inNormal;
    output.outTexCoord = input.inTexCoord;
    //out.normalMapEnabled = true;
    return output;
}