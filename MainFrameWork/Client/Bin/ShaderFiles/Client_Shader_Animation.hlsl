#ifndef _CLIENT_SHADER_FUNCTIONS_HLSL_
#define _CLIENT_SHADER_FUNCTIONS_HLSL_

#include "Client_Shader_Defines.hlsl"

#define MAX_INSTANCE 500

struct tagKeyframeDesc
{
    int animIndex;
    uint currFrame;
    uint nextFrame;
    float ratio;
    float sumTime;
    float speed;
    float2 padding;
};

struct tagTweenFrameDesc
{
    float tweenDuration;
    float tweenRatio;
    float tweenSumTime;
    float padding;
    tagKeyframeDesc curr;
    tagKeyframeDesc next;
};

struct tagTweenBuffer
{
    tagTweenFrameDesc TweenFrames[MAX_INSTANCE];
};

tagTweenBuffer g_TweenInstances;

Texture2DArray g_TransformMap;

int g_iSocketBoneIndex;

// 소켓일 경우
// vBlendIndex = float4(g_iSocketBoneIndex, 0.f, 0.f, 0.f)
// vBlendWeights = float4(1.f, 0.f, 0.f, 0.f)
matrix GetAnimationMatrix(tagTweenFrameDesc TweenFrame, float4 vBlendIndex, float4 vBlendWeights)
{
    float indices[4] = { vBlendIndex.x, vBlendIndex.y, vBlendIndex.z, vBlendIndex.w };
    float weights[4] = { vBlendWeights.x, vBlendWeights.y, vBlendWeights.z, vBlendWeights.w };

    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];
    
    animIndex[0] = TweenFrame.curr.animIndex;
    currFrame[0] = TweenFrame.curr.currFrame;
    nextFrame[0] = TweenFrame.curr.nextFrame;
    ratio[0] = TweenFrame.curr.ratio;
    
    animIndex[1] = TweenFrame.next.animIndex;
    currFrame[1] = TweenFrame.next.currFrame;
    nextFrame[1] = TweenFrame.next.nextFrame;
    ratio[1] = TweenFrame.next.ratio;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;

    for (int i = 0; i < 4; i++)
    {
        if (0.f == weights[i])
            continue;
        
        // TODO: 텍스쳐 포맷 변경 후 아래 테스트
        // c0 = g_TransformMap.Sample(PointSampler, float3(currFrame[0], animIndex[0], indices[i] * 4 + 0));
        
        c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
        c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
        c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
        c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
        curr = matrix(c0, c1, c2, c3);
        
        n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
        n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
        n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
        n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        matrix result = lerp(curr, next, ratio[0]);
        
        if (animIndex[1] >= 0)
        {
            c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
            c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
            c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
            c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
            curr = matrix(c0, c1, c2, c3);

            n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
            n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
            n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
            n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
            next = matrix(n0, n1, n2, n3);

            matrix nextResult = lerp(curr, next, ratio[1]);
            result = lerp(result, nextResult, TweenFrame.tweenRatio);
        }
        
        transform += mul(weights[i], result);
    }

    return transform;
}

#endif