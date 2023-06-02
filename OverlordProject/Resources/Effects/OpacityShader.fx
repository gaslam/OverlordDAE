float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float3 gLightDirection = float3(0.f, -1.f, 0.f);
float4 gColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

struct VS_INPUT {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord: TEXCOORD0;
};
struct VS_OUTPUT {
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord: TEXCOORD0;
};

RasterizerState FrontCulling
{
	CullMode = NONE;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

SamplerState gTextureSampler{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

float4 gColorDiffuse = float4(1,1,1,1);
bool gUseTextureDiffuse = true;
bool gTextureOpacityIntensity = false;
float gOpacityIntensity = 1.0f;
Texture2D gTextureDiffuse;
Texture2D gTextureOpacity;

float3 CalculateDiffuse(float3 normal,float2 texCoord)
{
		float diffuseStrength = saturate(dot(normal,-gLightDirection) * 0.5 + 0.5); 
	
	float3 diffuseColor = gColorDiffuse; 
	
	float3 textureColor = float3(0,0,0);
	if(gUseTextureDiffuse)
	{
		textureColor = gTextureDiffuse.Sample(gTextureSampler,texCoord) * diffuseStrength;
	}
	diffuseColor*=diffuseStrength;
	return diffuseColor * textureColor;
}

float CalculateOpacity(float2 texCoord)
{
	float opacity = gOpacityIntensity;
	if(gTextureOpacityIntensity)
	{
		opacity = gTextureOpacity.Sample(gTextureSampler,texCoord).r * gOpacityIntensity;
	}
	return opacity;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input) {
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.Position = mul(float4(input.Position, 1.0), gWorldViewProj);
	output.Normal = mul(input.Normal, (float3x3)gWorld);
	output.TexCoord = input.TexCoord;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float3 diffuse = CalculateDiffuse(input.Normal,input.TexCoord);
    float opacity = CalculateOpacity(input.TexCoord);
    if (opacity < 1.f)
        discard;
	return float4(diffuse,opacity);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
// Default Technique
technique10 WithAlphaBlending {
	pass p0 {
		SetRasterizerState(FrontCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}

// Default Technique
technique10 WithoutAlphaBlending {
	pass p0 {
		SetRasterizerState(FrontCulling);
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}
