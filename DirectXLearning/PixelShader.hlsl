
Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
    float4 pos: SV_POSITION0;
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

	float m_ambiantReflection;
	float3 m_ambiantLightColor;
	float m_diffuseLight;
	float m_specularLight;
	float m_shininess;
};


float4 psmain(PS_INPUT input) : SV_TARGET
{
	//return float4(255.0f, 240.0f, 0.0f, 1.0f); // the safety yellow

	float ka = m_ambiantReflection; // ambiant reflection constant
	float3 ia = m_ambiantLightColor; // ambiant light color

	float3 ambientLight = ka * ia;

	// diffuse light
	float kd = m_diffuseLight;
	float id = float3(1.0, 1.0, 1.0);
	float amountDiffuseLight = max(0.0, dot(m_lightDirection.xyz, input.normal));

	float3 diffuseLight = kd * amountDiffuseLight * id;

	// specular light
	float ks = m_specularLight; // constant specular reflection
	float3 is = float3(1.0, 1.0, 1.0);// specular light color
	float3 reflectedLight = reflect(m_lightDirection.xyz, input.normal);
	float shininess = m_shininess;
	float amountSpecularLight = pow(max(0.0, dot(reflectedLight, input.directionToCamera)), shininess);

	float specularLight = ks * amountSpecularLight * is;
	
	float3 finalLight = ambientLight + diffuseLight + specularLight;

    return float4(finalLight, 1.0);
}