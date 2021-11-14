Texture2D tex : register(t0);
SamplerState splr;

float4 main(float2 texCoords : TexCoords) : SV_Target
{
    return tex.Sample(splr, texCoords);
}