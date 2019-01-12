cbuffer CBuffer0
{
	matrix WVPMatrix; //64 bytes
};



struct VOut
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;//Note the spelling of this and all instances below
	float3 texcoord : TEXCOORD;
	float3 normal	: NORMAL;
};

TextureCube texture0;
SamplerState sampler0;

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal: NORMAL)
{
	VOut output;

	//color.r *= red_fraction;
    output.texcoord = position.xyz;
	output.position = position;// *scale;
	output.position = mul(WVPMatrix, position);
	output.normal = normal;
	output.normal = normalize(normal);


	output.color = color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD) : SV_TARGET
{
	return color * texture0.Sample(sampler0, texcoord);
}
