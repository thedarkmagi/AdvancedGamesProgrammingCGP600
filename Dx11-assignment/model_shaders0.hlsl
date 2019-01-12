cbuffer CBuffer0
{
	matrix WVPMatrix; //64 bytes
    matrix WorldView;
	float4 directional_light_vector; // 16 bytes;
	float4 directional_light_colour; // 16 bytes;
	float4 ambient_light_colour; // 16 bytes;
	bool twoTextures; // 2 bytes
};

Texture2D texture0;
TextureCube textureCube;
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

	//float4 default_color = { 1.0, 1.0, 1.0, 1.0 };
	
 //   float3 wvpos = mul(WorldView, position);

 //   float3 wvnormal = mul(WVPMatrix, normal);
 //   wvnormal = normalize(wvnormal);

 //   float3 eyer = -normalize(wvpos);
    

	output.position = mul(WVPMatrix, position);
	
    output.normal = mul(normal, (float3x3)WVPMatrix);
    
    float diffuse_amount = dot(directional_light_vector, normal);
    diffuse_amount = saturate(diffuse_amount);

    output.color = ambient_light_colour + (directional_light_colour * diffuse_amount);

   // output.texcoord = 2.0 * dot(eyer, wvnormal) * wvnormal - eyer;
	output.texcoord = texcoord;
	//output.color = default_color;

	return output;
}

float4 ModelPS(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal: NORMAL) : SV_TARGET
{
    // this results in some real jank
    // ATTEMPT AT SPECULAR 
    //float3 lightDir = normalize(normal - (float3) directional_light_vector);
    //float diffuseLighting = saturate(dot(normal, lightDir));
    //float3 ViewVector = float3(-1, 0, 0);

    //float4 worldPos = mul(position, WVPMatrix);
    //float3 h = normalize(normalize((float3) directional_light_vector - (float3) worldPos) + lightDir);

    //float specLighting = pow(saturate(dot(h, normal)), 5); // magic number is for specular power
    //float4 default_color = { 1.0, 1.0, 1.0, 1.0 };
    //color = saturate(ambient_light_colour + (directional_light_colour * diffuseLighting * 10)
    //+ (default_color * specLighting *10));


	return  texture0.Sample(sampler0, texcoord) * color;
	
}
