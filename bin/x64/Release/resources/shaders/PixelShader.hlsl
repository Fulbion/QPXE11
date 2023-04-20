Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 directionToCamera: TEXCOORD2;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_lightDirection;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float ka = 0.05;
	float3 ia = float3(1.0, 1.0, 1.0);

	float3 ambientLight = ka * ia;

	float kd = 0.8;
	float3 id = float3(1.0, 1.0, 1.0);
	float amountDiffuseLight = max(0.0, dot(m_lightDirection.xyz, input.normal));

	float3 diffuseLight = kd * id * amountDiffuseLight;

	float ks = 1.0;
	float3 is = float3(1.0, 1.0, 1.0);
	float3 reflectedLight = reflect(m_lightDirection.xyz, input.normal);
	float shininess = 30.0;
	float amountSpecularLight = pow(max(0.0, dot(reflectedLight, input.directionToCamera)), shininess);

	float3 specularLight = ks * is * amountSpecularLight;

	float3 finalLight = ambientLight + diffuseLight + specularLight;

	// return Texture.Sample(TextureSampler, input.texcoord / 2);
	return float4(finalLight, 1.0);
}