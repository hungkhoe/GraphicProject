struct INPUT_GEOMETRY
{
	float4 coordinate : SV_POSITION;
	float4 colorOut : COLOR;
	float2 uv : UV;
	float4 normals : NORMAL;
	float4 worldPos : WORLD_POS;
};

cbuffer ConstantMatrixBuffer : register(b0)
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

[maxvertexcount(4)]
void main(point float4 input[1] : SV_POSITION, inout TriangleStream<INPUT_GEOMETRY> triStream)
{
	INPUT_GEOMETRY geo[4];

	geo[0].coordinate = float4(-6, 4, 1, 1);
	geo[0].uv = float2(1, 1);
	geo[0].normals = float4(0, 0, -1, 0);
	geo[0].worldPos.xyz = geo[0].coordinate.xyz;
	geo[0].worldPos.w = 1;

	geo[1].coordinate = float4(-6, 7, 1, 1);
	geo[1].uv = float2(1, 0);
	geo[1].normals = float4(0, 0, -1, 0);
	geo[1].worldPos.xyz = geo[1].coordinate.xyz;
	geo[1].worldPos.w = 1;

	geo[2].coordinate = float4(-3, 4, 1, 1);
	geo[2].uv = float2(0, 1);
	geo[2].normals = float4(0, 0, -1, 0);
	geo[2].worldPos.xyz = geo[2].coordinate.xyz;
	geo[2].worldPos.w = 1;

	geo[3].coordinate = float4(-3, 7, 1, 1);
	geo[3].uv = float2(0, 0);
	geo[3].normals = float4(0, 0, -1, 0);
	geo[3].worldPos.xyz = geo[3].coordinate.xyz;
	geo[3].worldPos.w = 1;

	for (unsigned int i = 0; i < 4; i++) {
		float4 local = geo[i].coordinate;

		local = mul(local, WorldMatrix);
		local = mul(local, ViewMatrix);
		local = mul(local, ProjectionMatrix);

		geo[i].coordinate = local;
		triStream.Append(geo[i]);
	}
}