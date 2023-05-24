//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = MIRROR;
    AddressV = MIRROR;
};

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState NoDepth
{
    DepthEnable = TRUE;
};
/// Create Rasterizer State (Backface culling)
RasterizerState BackCulling
{
    CullMode = BACK;
};

Texture2D gTexture;

float2 gPixelAmount = float2(1920.f/2.f, 1080.f/2.f);

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
/// Create Rasterizer State (Backface culling) 


//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD1;
};


float4 pixelate(float2 texCoord)
{
    float2 newUV = floor(texCoord * gPixelAmount) / gPixelAmount;
    float4 color = gTexture.Sample(samPoint, newUV);
    return color;
}


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
	// Set the Position
	// Set the TexCoord
    output.Position = float4(input.Position, 1.f);
    output.TexCoord = input.TexCoord;
    return output;
}
float4 PS(PS_INPUT input) : SV_Target
{
    float4 finalColor = pixelate(input.TexCoord);
    return finalColor;
}


//TECHNIQUE
//---------
technique11 DefaultTechnique
{
    pass P0
    {
		// Set states...
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}