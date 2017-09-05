


struct OUTPUT_VERTEX
{
	float4 coordinate : SV_POSITION;
	float4 colorOut : COLOR0;
	float2 uv : UV0;
	
};

cbuffer ConstantMatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
	float4 constantColor;
}

// Pixel Shader
float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	return input.colorOut;

}

