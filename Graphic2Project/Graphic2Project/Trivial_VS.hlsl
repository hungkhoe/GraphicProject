struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 color : COLOR;
	float2 uv:  UV;

};

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
}

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;
	output.coordinate = float4(input.coordinate.xyz, 1);

	output.coordinate= mul(output.coordinate, WorldMatrix);
	
	output.coordinate = mul(output.coordinate, ViewMatrix);
	output.coordinate = mul(output.coordinate, ProjectionMatrix);	

	output.colorOut = input.color;
	output.uv = input.uv;


	return output;
}

