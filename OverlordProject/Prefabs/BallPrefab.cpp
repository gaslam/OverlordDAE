#include "stdafx.h"
#include "BallPrefab.h"

#include "Materials/UberMaterial.h"

BallPrefab::BallPrefab(const XMFLOAT4& color, const float scale) :
m_Scale{scale},
m_Color{color}
{
}

void BallPrefab::Initialize(const SceneContext&)
{
	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Shapes/Sphere.ovm"} };
	AddComponent(modelComponent);
	auto pMat{ MaterialManager::Get()->CreateMaterial<UberMaterial>() };
	XMFLOAT4 colorWhite{ XMFLOAT4{ Colors::White } };
	pMat->SetColorDiffuse(m_Color);
	pMat->SetColorSpecular(colorWhite);
	pMat->SetShininess(20);
	pMat->SetUseSpecularBlinn(true);
	pMat->SetUseDiffuseTexture(false);
	modelComponent->SetMaterial(pMat);

	GetTransform()->Scale(m_Scale);
}
