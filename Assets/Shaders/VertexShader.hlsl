struct VSOut
{
    float2 texCoords : TexCoords;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    matrix transform;
};

VSOut main(float3 pos : Position, float2 texCoords : TexCoords)
{
    VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), transform );
    vso.texCoords = texCoords;
    return vso;
}