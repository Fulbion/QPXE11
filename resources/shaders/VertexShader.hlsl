struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: TEXCOORD1;
	float3 directionToCamera: TEXCOORD2;
};


cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_lightDirection;
	float4 m_cameraPosition;
};



VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	// output.position = lerp(input.position, input.position1, (float)((sin((float)(m_time / (float)1000.0f)) + 1.0f) / 2.0f));

	output.position = mul(input.position, m_world);
	output.directionToCamera = normalize(output.position.xyz - m_cameraPosition.xyz);
	output.position = mul(output.position, m_view);
	output.position = mul(output.position, m_proj);

	output.texcoord = input.texcoord;
	output.normal = input.normal;
	return output;
}