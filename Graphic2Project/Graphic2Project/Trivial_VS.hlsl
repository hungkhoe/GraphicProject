struct INPUT_VERTEX
{
	float3 coordinate : POSITION;
	float2 textureUV : TEXTURE;
	float4 color : COLOR;
};

struct OUTPUT_VERTEX
{
	float4 coordinate : POSITION;
	float2 textureUV : TEXTURE;	
	float4 color : COLOR;
};

cbuffer FIRST_VRAM : register( b1 )
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

OUTPUT_VERTEX main( INPUT_VERTEX input )
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;

	output.coordinate.xyz = input.coordinate;

	output.coordinate.w = 1;
		
	// TODO : PART 4 STEP 4
	output.coordinate = mul(output.coordinate,World);

	output.coordinate = mul(output.coordinate, View);

	output.coordinate = mul(output.coordinate, Projection);

	output.textureUV = input.textureUV;

	return output;
}