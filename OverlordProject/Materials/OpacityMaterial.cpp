#include "stdafx.h"
#include "OpacityMaterial.h"

OpacityMaterial::OpacityMaterial() : Material<OpacityMaterial>(L"Effects/OpacityShader.fx")
{
}

void OpacityMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void OpacityMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacityTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void OpacityMaterial::SetUseOpacityMap(const bool canUseOpacity)
{
	m_UseOpacityMap = canUseOpacity;
	InitializeEffectVariables();
}

void OpacityMaterial::InitializeEffectVariables()
{
	if (m_pDiffuseTexture)
	{
		SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture->GetShaderResourceView());
	}

	if (m_pOpacityTexture)
	{
		SetVariable_Texture(L"gTextureOpacity", m_pOpacityTexture->GetShaderResourceView());
	}

	SetVariable_Scalar(L"gTextureOpacityIntensity", m_UseOpacityMap);
}