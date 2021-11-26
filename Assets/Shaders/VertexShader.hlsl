struct VSOut
{
    float2 texCoords : TexCoords;
    float4 pos : SV_Position;
};

cbuffer CBuf : register (b0)
{
    matrix proj;
};

cbuffer CBuf : register (b1)
{
    matrix model;
};

VSOut main(float3 pos : Position, float2 texCoords : TexCoords)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), proj );
    vso.texCoords = texCoords;
    return vso;
}