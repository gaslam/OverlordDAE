#include "stdafx.h"
#include "ParticleMaterial.h"

ParticleMaterial::ParticleMaterial():
Material(L"Effects/ParticleRenderer.fx")
{
}

void ParticleMaterial::InitializeEffectVariables()
{
}

void ParticleMaterial::SetVariableWorldViewProj(const float* pData)
{
	SetVariable_Matrix(L"gWorldViewProj", pData);
}

void ParticleMaterial::SetVariableViewInv(const float* pData)
{
	SetVariable_Matrix(L"gViewInverse", pData);
}

void ParticleMaterial::SetVariableParticleTexture(TextureData* data)
{
	SetVariable_Texture(L"gDiffuseMap", data->GetShaderResourceView());
}
