#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/UberMaterial.h"

UberMaterialScene::UberMaterialScene() : GameScene(L"UberMaterialScene")
{
}

void UberMaterialScene::Initialize()
{
	m_pSphere = new GameObject{};
	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Shapes/Sphere.ovm"} };
	m_pSphere->AddComponent(modelComponent);
	auto pUberMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pUberMat->SetUseDiffuseTexture(true);
	pUberMat->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	m_UberMatId = pUberMat->GetMaterialId();
	LoadScene();
	pUberMat->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	pUberMat->SetOpacityMapTexture(L"Textures/Skulls_Heightmap.tga");

	modelComponent->SetMaterial(pUberMat);

	AddChild(m_pSphere);
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;


	m_SceneContext.pInput->AddInputAction(InputAction{ Inputs::reset,InputState::pressed,'R' });
	
}

void UberMaterialScene::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(Inputs::reset))
	{
		LoadScene();
	}
}

void UberMaterialScene::Draw()
{
}

void UberMaterialScene::OnGUI()
{
	auto pUberMat = MaterialManager::Get()->GetMaterial<UberMaterial>(m_UberMatId);
	if (!pUberMat)
	{
		return;
	}
	pUberMat->DrawImGui();
}

void UberMaterialScene::LoadScene()
{
	auto pUberMat = MaterialManager::Get()->GetMaterial<UberMaterial>(m_UberMatId);
	if (!pUberMat)
	{
		return;
	}

	XMFLOAT4 colorRed{ XMFLOAT4{ Colors::Red } };
	XMFLOAT4 colorBlack{ XMFLOAT4{ Colors::Black } };
	XMFLOAT4 colorWhite{ XMFLOAT4{ Colors::White } };
	int shininess{ 100 };
	float reflectionStrength{ 0.16f };
	float fresnelMultiplier{ 1.f };
	float opacityIntensity{ 1.f };
	pUberMat->SetColorDiffuse(colorRed);
	pUberMat->SetColorSpecular(colorBlack);
	pUberMat->SetShininess(shininess);
	pUberMat->SetColorAmbient(colorRed);
	pUberMat->SetFlipGreenChannel(false);
	pUberMat->SetUseTextureNormal(true);
	pUberMat->SetUseEnvMapping(true);
	pUberMat->SetReflectionStrength(reflectionStrength);
	pUberMat->SetColorFresnel(colorWhite);
	pUberMat->SetFresnelMultiplier(fresnelMultiplier);
	pUberMat->SetOpacityIntensity(opacityIntensity);
	pUberMat->SetUseSpecularPhong(true);

	auto pGameObject = m_SceneContext.pCamera->GetGameObject();
	if (!pGameObject)
	{
		Logger::LogError(L"Camera GameObject not found!!");
	}

	auto pCamera = dynamic_cast<FreeCamera*>(pGameObject);
	if (!pCamera)
	{
		Logger::LogError(L"Camera not found!!");
		return;
	}
	pCamera->SetRotation(30.f, 0.f);
	auto pTransform = pCamera->GetTransform();
	if (!pTransform)
	{
		Logger::LogError(L"Camera transform not found!!");
		return;
	}
	pTransform->Translate(0.f, 4.f, -6.f);
}

void UberMaterialScene::OnSceneActivated()
{
	LoadScene();
}
