
Texture2D texture1: register(t0);
SamplerState samplerState1 : register(s0);

struct OUTPUT_VERTEX
{
	float4 coordinate : POSITION;
	float2 textureUV : TEXTURE;
	float4 color : COLOR;
};

cbuffer FIRST_VRAM : register(b1)
{
	matrix View;
}

cbuffer SECOND_VRAM : register (b2)
{
	matrix Projection;
}

cbuffer THIRD_VRAM : register (b3)
{
	matrix World;
	float4 meshColor;
}

float4 main(OUTPUT_VERTEX outputRaster) : SV_TARGET
{
	return outputRaster.color;
}