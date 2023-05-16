//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

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


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	// Set the Position
	// Set the TexCoord
	output.Position = float4(input.Position,1.f);
	output.TexCoord = input.TexCoord;
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	float w;
	float h;
	// Step 1: find the dimensions of the texture (the texture has a method for that)	
	gTexture.GetDimensions(w,h);
	// Step 2: calculate dx and dy (UV space for 1 pixel)
	float wUV = 1.f/w;
	float hUV = 1.f/h;
	float4 finalColor;	
	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
	float2 offSet;
	for(int i = -2; i < 2; ++i)
	{
		for(int j = -2; j < 2; ++j)
		{
			offSet.x = i*wUV*2;
			offSet.y = j*hUV *2;
			finalColor += gTexture.Sample(samPoint,input.TexCoord + offSet);
		}
	}
	// Step 4: Divide the final color by the number of passes (in this case 5*5)
	finalColor.r /= 5*5;
	finalColor.g /= 5*5;
	finalColor.b /= 5*5;
	finalColor.a /= 5*5;
	// Step 5: return the final color

	return finalColor;
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}