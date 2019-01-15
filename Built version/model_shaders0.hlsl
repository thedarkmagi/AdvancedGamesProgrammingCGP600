cbuffer CBuffer0
{
	matrix WVPMatrix; //64 bytes
    matrix WorldView;
	float4 directional_light_vector; // 16 bytes;
	float4 directional_light_colour; // 16 bytes;
	float4 ambient_light_colour; // 16 bytes;
    float4 camPosition;
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
	
    output.normal = normalize(mul(normal, (float3x3) WVPMatrix));
    float diffuse_amount = dot((float3) directional_light_vector, normal);
    diffuse_amount = saturate(diffuse_amount);

    output.color = ambient_light_colour + (directional_light_colour * diffuse_amount);

   // output.texcoord = 2.0 * dot(eyer, wvnormal) * wvnormal - eyer;
	output.texcoord = texcoord;
	//output.color = default_color;

	return output;
}

float4 ModelPS(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL) : SV_TARGET
{

	//float diffuse_amount = dot((float3) directional_light_vector, normal);
	//diffuse_amount = saturate(diffuse_amount);

	//color = ambient_light_colour + (directional_light_colour * diffuse_amount);


    float4 default_color = { 1.0, 1.0, 1.0, 1.0 };

    float4 finalColour = color;
    float3 toEye = camPosition.xyz - position.xyz;
    toEye = normalize(toEye);
    float3 dirToLight = directional_light_vector.xyz - toEye;
    //phong diffuse 
    float NDotL = dot(dirToLight, normal);
    finalColour += default_color * saturate(NDotL);

    // Blinn specular
    float3 halfWay = normalize(dirToLight -toEye  );
    float NDotH = saturate(dot(halfWay, normal));
    finalColour += default_color * pow(NDotH, 2) * 2;
    return texture0.Sample(sampler0, texcoord) *  finalColour;
	
}
