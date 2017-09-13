
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

cbuffer SpotLight:register (b2)
{
	float4 spotLightPosition;
	float4 spotLightColor;
	float4 spotLightRadius;
	float4 spotLightDirection;
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
	pointLightAttenuation *= pointLightAttenuation;
	float4 finalPointLight = pointLightratio * lightPointColor * baseColor * pointLightAttenuation;

	// Spot Light // 
	float4 LightDirection = normalize(spotLightPosition - input.worldPos);
	LightDirection.w = 0;

	float surfaceRatio = saturate(dot(-LightDirection, spotLightDirection));
	float spotfactor = (surfaceRatio > spotLightRadius.y) ? 1 : 0;
	float spotLightRatio = saturate(dot(LightDirection, input.normal));
	float spotLightCone = 1.0f - saturate((spotLightRadius.x - surfaceRatio) / (spotLightRadius.x - spotLightRadius.y));	
	float4 finalSpotLight = spotfactor * spotLightRatio * spotLightColor * baseColor * spotLightCone;
	
	baseColor = saturate(ambient + finalDirectionalLight + finalPointLight + finalSpotLight);

	return (baseColor * tex.Sample(samp, input.uv));
}

