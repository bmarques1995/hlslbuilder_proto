#pragma pack_matrix( column_major )

cbuffer u_Matrices : register(b0)
{
    float4x4 VP;
    float4x4 M;
    float4 RecColor;
};

struct VS_In
{
    float3 pos : POSITION;
    float2 texCoord : TEXPOINT;
};

struct PS_In
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXPOINT;
};

Texture2D objTexture : u_TextureData : register(t0);
SamplerState objSampler : u_TextureSampler : register(s0);

PS_In vs_main(VS_In input)
{
    PS_In vso;
    vso.pos = float4(input.pos, 1.0f);
    vso.pos = mul(vso.pos, M);
    vso.pos = mul(vso.pos, VP);
    vso.texCoord = input.texCoord;
    vso.color = RecColor;
    return vso;
}

float4 ps_main(PS_In input) : SV_TARGET
{
    /*float4 color = objTexture.SampleLevel(objSampler, input.texCoord, 1.0f);*/
    /*float4 color = objTexture.SampleLevel(objSampler, input.texCoord, 0.0f);*/
    float4 color = objTexture.SampleLevel(objSampler, input.texCoord, 0.0f);
    return color;
}
