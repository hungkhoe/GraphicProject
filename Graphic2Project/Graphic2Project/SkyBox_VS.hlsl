struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
};

struct OUTPUT_VERTEX
{
	float4 coordinate : SV_POSITION;
	float3 textureCoord : TEXTURECOORD;
};

cbuffer ConstantMatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;

	output.coordinate = float4(input.coordinate.xyz, 1);

	output.coordinate = mul(output.coordinate, WorldMatrix);

	output.textureCoord = output.coordinate.xyz;

	output.coordinate = mul(output.coordinate, ViewMatrix);

	output.coordinate = mul(output.coordinate, ProjectionMatrix);	

	return output;
}

