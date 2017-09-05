
Texture2D tex : register(t0);
SamplerState samp : register(s0);

struct OUTPUT_VERTEX
{
	float4 coordinate : SV_POSITION;
	float4 colorOut : COLOR;
	float2 uv : UV;
	float4 normal :NORMAL;
};

cbuffer DirectionalLight: register (b0)
{
	float3 direction;
	float padding;
	float4 color;
}

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float4 baseColor = tex.Sample(samp,input.uv);
	
	//ambient
	float4 ambient = float4(0.25, 0.25, 0.25, 1);

	//directional light
	float directionalLightRatio = saturate(dot(-normalize(direction), normalize(input.normal)));
	float4 finalDirectionalLight = directionalLightRatio * color * baseColor;
	
	baseColor = saturate(ambient + finalDirectionalLight);

	return (baseColor * tex.Sample(samp, input.uv));
}

