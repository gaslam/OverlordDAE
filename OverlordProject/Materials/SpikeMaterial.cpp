#include "stdafx.h"
#include "SpikeMaterial.h"


SpikeMaterial::SpikeMaterial() : Material<SpikeMaterial>(L"Effects/SpikyShader.fx")
{

}
void SpikeMaterial::SetDiffuseColor(const XMFLOAT4& color)
{
	SetVariable_Vector(L"gColorDiffuse", color);
}

void SpikeMaterial::SetSpikeLength(float length)
{
	SetVariable_Scalar(L"gSpikeLength", length);
}

void SpikeMaterial::InitializeEffectVariables()
{
	const XMFLOAT4 colorRed{ XMFLOAT4{Colors::Red} };
	SetDiffuseColor(colorRed);
}
