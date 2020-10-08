cbuffer alphaBuffer : register(b1)
{
    float alpha;
};

struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    //float3 inNormal : NORMAL;
    float2 inTexCoord : TEXCOORD;
    //bool normalMapEnabled : NORMAL_MAP_ENABLED;
};

Texture2D tex : TEXTURE : register(t0);
//Texture2D normalMap : NORMAL_MAP : register(t1);
SamplerState samplerState : SAMPLER : register(s0);

float4 PS_main(PS_INPUT input) : SV_TARGET
{
    /*
    if (input.normalMapEnabled)
    {
        const float3 normalSample = normalMap.Sample(samplerState, input.inTexCoord).xyz;
        input.inNormal.x = normalSample.x * 2.0f - 1.0f;
        input.inNormal.y = normalSample.y * 2.0f - 1.0f;
        input.inNormal.z = normalSample.z;
    }
*/
    
    float3 pixelColour = tex.Sample(samplerState, input.inTexCoord).xyz;
    
    //return float4(pixelColour, alpha);
    return float4(1.0f, 1.0f, 1.0f, alpha);
}