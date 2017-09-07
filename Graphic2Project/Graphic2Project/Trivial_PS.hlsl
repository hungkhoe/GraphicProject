
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

cbuffer DirectionalLight: register (b0)
{
	float3 direction;
	float padding;
	float4 color;
}

cbuffer PointLight:register (b1)
{
	float4 lightPointPostion;
	float4 lightPointColor;	
	float4 lightPointRadius;
}

float4 main(OUTPUT_VERTEX input) : SV_TARGET
{
	float4 baseColor = tex.Sample(samp,input.uv);
	
	//ambient
	float4 ambient = float4(0.1f, 0.1f, 0.1f, 1);

	//directional light
	float directionalLightRatio = saturate(dot(-normalize(direction), normalize(input.normal)));
	float4 finalDirectionalLight = directionalLightRatio * color * baseColor;

	//point light
	float4 pointLightDirection = normalize(lightPointPostion - input.worldPos);
	float pointLightratio = saturate(dot(pointLightDirection, input.normal));
	float pointLightAttenuation = 1.0 - saturate(length(lightPointPostion - input.worldPos) / 8);

	//pointLightAttenuation *= pointLightAttenuation;
	//pointLightAttenuation *= pointLightAttenuation;
	pointLightAttenuation *= pointLightAttenuation;

	float4 finalPointLight = pointLightratio * lightPointColor * baseColor * pointLightAttenuation;
	
	baseColor = saturate(ambient + finalDirectionalLight + finalPointLight);

	return (baseColor * tex.Sample(samp, input.uv));
}

