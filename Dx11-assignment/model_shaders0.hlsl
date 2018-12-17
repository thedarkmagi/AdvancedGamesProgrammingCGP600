cbuffer CBuffer0
{
	matrix WVPMatrix; //64 bytes
	float4 directional_light_vector; // 16 bytes;
	float4 directional_light_colour; // 16 bytes;
	float4 ambient_light_colour; // 16 bytes;
	bool twoTextures; // 2 bytes
};

Texture2D texture0;

SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
	float4 color	: COLOR; //Note the spelling of this and all instances below
	float2 texcoord : TEXCOORD;
	float3 normal	: NORMAL;
};



VOut ModelVS(float4 position : POSITION, float2 texcoord : TEXCOORD, float3 normal: NORMAL)
{
	VOut output;

	float4 default_color = { 1.0, 1.0, 1.0, 1.0 };
	
	output.position = mul(WVPMatrix, position);
	float diffuse_amount = dot(directional_light_vector, normal);
	diffuse_amount = saturate(diffuse_amount);

	output.color = ambient_light_colour + (directional_light_colour * diffuse_amount);

	output.texcoord = texcoord;
	//output.color = default_color;

	return output;
}

float4 ModelPS(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{

	return  texture0.Sample(sampler0, texcoord) * color;
	
}
