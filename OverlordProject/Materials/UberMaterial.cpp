#include "stdafx.h"
#include "UberMaterial.h"

using namespace MathHelper;

UberMaterial::UberMaterial() : Material<UberMaterial>(L"Effects/Uber.fx")
{
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void UberMaterial::SetSpecularLevelTexture(const std::wstring& assetFile)
{
	m_pSpecularLvlTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void UberMaterial::SetEnvironmentMapTexture(const std::wstring& assetFile)
{
	m_pEnvironmentMapTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void UberMaterial::SetOpacityMapTexture(const std::wstring& assetFile)
{
	m_pOpacityMapTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEffectVariables();
}

void UberMaterial::SetUseDiffuseTexture(bool value)
{
	m_UseDiffuseTexture = value;
	InitializeEffectVariables();
}

void UberMaterial::SetFlipGreenChannel(bool value)
{
	m_FlipGreenChannel = value;
	InitializeEffectVariables();
}

void UberMaterial::SetUseFresnelFalloff(bool value)
{
	m_UseFresnelFalloff = value;
	InitializeEffectVariables();
}

void UberMaterial::SetUseTextureOpacity(bool value)
{
	m_UseTextureOpacityIntensity = value;
	InitializeEffectVariables();
}


void UberMaterial::SetUseEnvMapping(bool value)
{
	m_UseEnvMapping = value;
	InitializeEffectVariables();
}

void UberMaterial::SetUseSpecularBlinn(bool value)
{
	m_UseSpecularBlinn = value;
	InitializeEffectVariables();
}

void UberMaterial::SetUseSpecularPhong(bool value)
{
	m_UseSpecularPhong = value;
	InitializeEffectVariables();
}

void UberMaterial::SetUseTextureNormal(bool value)
{
	m_UseTextureNormal = value;
	InitializeEffectVariables();
}

void UberMaterial::SetShininess(int value)
{
	Clamp<int>(value, 100, 0);
	m_Shininess = value;
	InitializeEffectVariables();
}

void UberMaterial::SetAmbientIntensity(float value)
{
	m_AmbientIntensity = value;
	Clamp<float>(value, 1.f, 0.f);
	InitializeEffectVariables();
}

void UberMaterial::SetReflectionStrength(float value)
{
	m_ReflectionStrength = value;
	Clamp<float>(value, 1.f, 0.f);
	InitializeEffectVariables();
}

void UberMaterial::SetRefractionStrength(float value)
{
	Clamp<float>(value, 1.f, 0.f);
	m_RefractionStrength = value;
	InitializeEffectVariables();
}

void UberMaterial::SetFresnelPower(float value)
{
	Clamp<float>(value, 100.f, 0.f);
	m_FresnelPower = value;
	InitializeEffectVariables();
}

void UberMaterial::SetFresnelMultiplier(float value)
{
	Clamp<float>(value, 100.f, 0.f);
	m_FresnelMultiplier = value;
	InitializeEffectVariables();
}

void UberMaterial::SetFresnelHardness(float value)
{
	Clamp<float>(value, 100.f, 0.f);
	m_FresnelHardness = value;
	InitializeEffectVariables();
}

void UberMaterial::SetOpacityIntensity(float index)
{
	Clamp<float>(index, 1.f, 0.f);
	m_OpacityIntensity = index;
	InitializeEffectVariables();
}

void UberMaterial::SetRefractionIndex(float index)
{
	Clamp<float>(index, 1.f, 0.f);
	m_RefractionIndex = index;
	InitializeEffectVariables();
}

void UberMaterial::SetColorDiffuse(XMFLOAT4& color)
{
	m_ColorDiffuse = color;
	InitializeEffectVariables();
}

void UberMaterial::SetColorSpecular(XMFLOAT4& color)
{
	m_ColorSpecular = color;
	InitializeEffectVariables();
}

void UberMaterial::SetColorAmbient(XMFLOAT4& color)
{
	m_ColorAmbient = color;
	InitializeEffectVariables();
}

void UberMaterial::SetColorFresnel(XMFLOAT4& color)
{
	m_ColorFresnel = color;
	InitializeEffectVariables();
}

void UberMaterial::InitializeEffectVariables()
{
	if (m_pDiffuseTexture)
	{
		SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture->GetShaderResourceView());
	}

	if (m_pSpecularLvlTexture)
	{
		SetVariable_Texture(L"gTextureSpecularIntensity", m_pSpecularLvlTexture->GetShaderResourceView());
	}

	if (m_pNormalTexture)
	{
		SetVariable_Texture(L"gTextureNormal", m_pNormalTexture->GetShaderResourceView());
	}

	if (m_pEnvironmentMapTexture)
	{
		SetVariable_Texture(L"gCubeEnvironment", m_pEnvironmentMapTexture->GetShaderResourceView());
	}
	if (m_pOpacityMapTexture)
	{
		SetVariable_Texture(L"gTextureOpacity", m_pOpacityMapTexture->GetShaderResourceView());
	}

	SetVariable_Scalar(L"gUseTextureDiffuse", m_UseDiffuseTexture);
	SetVariable_Scalar(L"gShininess", m_Shininess);
	SetVariable_Scalar(L"gAmbientIntensity", m_AmbientIntensity);
	SetVariable_Scalar(L"gReflectionStrength", m_ReflectionStrength);
	SetVariable_Scalar(L"gRefractionStrength", m_RefractionStrength);
	SetVariable_Scalar(L"gRefractionIndex", m_RefractionIndex);
	SetVariable_Scalar(L"gFresnelPower", m_FresnelPower);
	SetVariable_Scalar(L"gFlipGreenChannel", m_FlipGreenChannel);
	SetVariable_Scalar(L"gUseTextureNormal", m_UseTextureNormal);
	SetVariable_Scalar(L"gUseEnvironmentMapping", m_UseEnvMapping);
	SetVariable_Scalar(L"gUseFresnelFalloff", m_UseFresnelFalloff);
	SetVariable_Scalar(L"gTextureOpacityIntensity", m_UseTextureOpacityIntensity);
	SetVariable_Scalar(L"gUseSpecularBlinn", m_UseSpecularBlinn);
	SetVariable_Scalar(L"gUseSpecularPhong", m_UseSpecularPhong);
	SetVariable_Scalar(L"gFresnelPower", m_FresnelPower);
	SetVariable_Scalar(L"gFresnelMultiplier", m_FresnelMultiplier);
	SetVariable_Scalar(L"gFresnelHardness", m_FresnelHardness);
	SetVariable_Scalar(L"gOpacityIntensity", m_OpacityIntensity);

	SetVariable_Vector(L"gColorDiffuse", m_ColorDiffuse);
	SetVariable_Vector(L"gColorSpecular", m_ColorSpecular);
	SetVariable_Vector(L"gColorAmbient", m_ColorAmbient);
	SetVariable_Vector(L"gColorFresnel", m_ColorFresnel);
}
