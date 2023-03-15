#include "stdafx.h"
#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial() : Material<DiffuseMaterial>(L"Effects/PosNormTex3D.fxc")
{
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void DiffuseMaterial::InitializeEffectVariables()
{
	if (m_pDiffuseTexture)
	{
		SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture->GetShaderResourceView());
	}
}
