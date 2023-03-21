//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gMatrixWorld : WORLD;
float3 gColorDiffuse : COLOR = float3(1.0, 1.0, 1.0);

float gSpikeLength<
	string UIName = "Spike Length";
	float UIMin = 0.f;
	float UIMax = 10.f;
	float UIStep = 0.1f;
> = 0.f;

float3 gLightDirection :DIRECTION
<
	string UIName = "Light Direction";
	string Object = "TargetLight";
> = float3(0.577f, 0.577f, 0.577f);


//STATES
//******
RasterizerState gRS_NoCulling { CullMode = NONE; };


//STRUCTS
//******
struct VS_DATA
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
};




//MAIN VERTEX SHADER
//******************
VS_DATA MainVS(VS_DATA vsData)
{
	GS_DATA temp = (GS_DATA)0;
	return vsData;
}


//MAIN PIXEL SHADER
//*****************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    input.Normal=normalize(input.Normal);
	float diffuseStrength = max(dot(normalize(gLightDirection),-input.Normal),0.2f); 
	return float4(gColorDiffuse*diffuseStrength,1.0f);
}

void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float3 normal)
{
		GS_DATA temp = (GS_DATA)0;
	temp.Position = mul(float4(pos, 1), gMatrixWorldViewProj);
    temp.Normal = mul(normal, (float3x3) gMatrixWorld);
	
	triStream.Append(temp);
}

[maxvertexcount(9)]
void SpikeGenerator(triangle VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{
	float3 center = (vertices[0].Position + vertices[1].Position + vertices[2].Position) / 3;
	float3 normal = normalize(vertices[0].Normal + vertices[1].Normal + vertices[2].Normal) / 3;
	center += gSpikeLength * normal;
	
	float3 v1 = (vertices[1].Position-vertices[0].Position);
	float3 v2 = (center - vertices[0].Position);
	float3 faceNormal = normalize(cross(v2,v1));
	
	CreateVertex(triStream,vertices[0].Position,faceNormal);
    CreateVertex(triStream,vertices[1].Position,faceNormal);
    CreateVertex(triStream,center,faceNormal);
	triStream.RestartStrip();
	
	v1 = (vertices[1].Position-vertices[2].Position);
	v2 = (center - vertices[2].Position);
	faceNormal = normalize(cross(v1,v2));
	
		CreateVertex(triStream,vertices[1].Position,faceNormal);
    CreateVertex(triStream,vertices[2].Position,faceNormal);
    CreateVertex(triStream,center,faceNormal);
	triStream.RestartStrip();
	
		v1 = (vertices[0].Position-vertices[2].Position);
	v2 = (center - vertices[2].Position);
	faceNormal = normalize(cross(v2,v1));
	
		CreateVertex(triStream,vertices[2].Position,faceNormal);
    CreateVertex(triStream,vertices[0].Position,faceNormal);
    CreateVertex(triStream,center,faceNormal);
}


//TECHNIQUES
//**********
technique10 Default
{
    pass p0
    {
        SetRasterizerState(gRS_NoCulling);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0,SpikeGenerator()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}