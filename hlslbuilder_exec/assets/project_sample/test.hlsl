#pragma pack_matrix( column_major )

#ifdef DXC_HLSL
    #define VK_LOCATION(n) [[vk::location=n]] 
#else
    #define VK_LOCATION(n)
#endif

struct VS_Input
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_Input vs_main(VS_Input inData)
{
    PS_Input outData;
    outData.pos = float4(inData.pos, 1.0f);
    outData.color = inData.color;
    return outData;
}

float4 ps_main(PS_Input inData) : SV_TARGET
{
    return inData.color;
}