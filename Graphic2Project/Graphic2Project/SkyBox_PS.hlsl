
TextureCube skyboxTexture : register(t0);
SamplerState samp : register(s0);

struct OUTPUT_VERTEX
{
	float4 coordinate : SV_POSITION;
	float3 textureCoord : TEXTURECOORD;
};	

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float4 baseColor = skyboxTexture.Sample(samp,input.textureCoord);

	return baseColor;
}

