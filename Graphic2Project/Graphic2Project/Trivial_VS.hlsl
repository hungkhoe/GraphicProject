struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 color : COLOR;
	float2 uv:  UV;
	float4 normals : NORMAL;

};

struct OUTPUT_VERTEX
{
	float4 coordinate : SV_POSITION;
	float4 colorOut : COLOR;
	float2 uv : UV;
	float4 normals : NORMAL;	
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

	output.uv = input.uv;

	output.normals = mul(float4(input.normals.xyz, 0), WorldMatrix);

	return output;
}

