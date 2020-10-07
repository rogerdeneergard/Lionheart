struct SKIN_INPUT
{
    float4 bindPos : POSITION;
    float4x4 boneOffset[4] : BONE_OFFSET;
    float4 boneWeight : BONE_WEIGHT;
};

StructuredBuffer<SKIN_INPUT> inputSkinData : register(t0);
RWStructuredBuffer<float4> outputVertexPos : register(u0);

[numthreads(8, 8, 1)]
void CS_skin(int3 dispatchThreadID : SV_DispatchThreadID)
{
    SKIN_INPUT input = inputSkinData[dispatchThreadID.x];
    
    float4 outPos = 0;
    
    float weights[4] = (float[4]) input.boneWeight;
    
    uint numInfluences = 0;
    
    for (int i = 0; i < 4; i++)
    {
        if (weights[i] != 0.0f)
            numInfluences += 1;
    }

    if (numInfluences == 0)
    {
        outPos == input.bindPos;
        return;
    }
    
    for (int j = 0; j < numInfluences; j++)
    {
        outPos += mul((weights[i] * input.bindPos), input.boneOffset[i]);
    }

    outputVertexPos[dispatchThreadID.x] = outPos;
}