#include "stdafx.h"
#include "ShadowMapMaterial.h"

ShadowMapMaterial::ShadowMapMaterial():
	Material(L"Effects/Shadow/ShadowMapGenerator.fx")
{
}

void ShadowMapMaterial::SetLightVP(const XMFLOAT4X4& lightViewProj)
{
	if (m_pLightViewProj)
	{
		m_pLightViewProj->SetMatrix(reinterpret_cast<const float*>(&lightViewProj));
	}
}

void ShadowMapMaterial::SetWorld(const XMFLOAT4X4& worldMatrix)
{
	if (m_pWorld)
	{
		m_pWorld->SetMatrix(reinterpret_cast<const float*>(&worldMatrix));
	}
}

void ShadowMapMaterial::SetBones(const float* pData, UINT count)
{
	if (m_pBones)
	{
		m_pBones->SetMatrixArray(pData, 0, count);
	}
}

void ShadowMapMaterial::InitializeEffectVariables()
{
	m_pWorld = GetVariable(L"gWorld")->AsMatrix();
	m_pLightViewProj = GetVariable(L"gLightViewProj")->AsMatrix();
	m_pBones =	GetVariable(L"gBones")->AsMatrix();
}
