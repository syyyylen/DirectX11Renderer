struct VS_INPUT
{
	float4 pos: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT
{
	float4 pos: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 directionToCamera: TEXCOORD1;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_projection;
	float4 m_lightDirection;
	float4 m_cameraPosition;
};


VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	// World space coordinates
	output.pos = mul(input.pos, m_world);
	output.directionToCamera = normalize(output.pos.xyz - m_cameraPosition.xyz);

	// View space
	output.pos = mul(output.pos, m_view);

	// Screen space
	output.pos = mul(output.pos, m_projection);
	
	output.texcoord = input.texcoord;
	output.normal = input.normal;
	return output;
}