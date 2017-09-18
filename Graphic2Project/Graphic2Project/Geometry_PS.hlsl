Texture2D tex : register(t0);
SamplerState samp : register(s0);

struct OUTPUT_VERTEX
{
	float4 coordinate : SV_POSITION;
	float4 colorOut : COLOR;
	float2 uv : UV;
	float4 normal :NORMAL;
	float4 worldPos : WORLD_POS;
};

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float4 baseColor = tex.Sample(samp,input.uv);

	return baseColor;
}
