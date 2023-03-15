#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial() : Material<UberMaterial>(L"Effects/PosNormTex3D.fxc")
{
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
}

void UberMaterial::SetSpecularLevelTexture(const std::wstring& assetFile)
{
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
}

void UberMaterial::SetEnvironmentMapTexture(const std::wstring& assetFile)
{
}

void UberMaterial::SetOpacityMapTexture(const std::wstring& assetFile)
{
}

void UberMaterial::InitializeEffectVariables()
{
}
