#include "stdafx.h"
#include "SpikeTestScene.h"
#include "Materials/SpikeMaterial.h"

void SpikeTestScene::Initialize()
{
	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Shapes/OctaSphere.ovm"} };
	m_pSphere = new GameObject{};
	m_pSphere->AddComponent<ModelComponent>(modelComponent);
	auto pTransform = m_pSphere->GetTransform();
	if (pTransform)
	{
		pTransform->Scale(15.f);
	}
	AddChild(m_pSphere);

	m_pSpikeMat = MaterialManager::Get()->CreateMaterial<SpikeMaterial>();
	XMFLOAT4 colorRed{ XMFLOAT4{Colors::Red} };
	m_pSpikeMat->SetDiffuseColor(colorRed);
	m_pSpikeMat->SetSpikeLength(2.f);
	modelComponent->SetMaterial(m_pSpikeMat);
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.pInput->AddInputAction(InputAction{ Inputs::reset,InputState::pressed,'R' });
}

void SpikeTestScene::Update()
{
	float deltaT = m_SceneContext.pGameTime->GetElapsed();
	m_DegreesTurned += deltaT * m_RotationSpeedY;
	m_pSphere->GetTransform()->Rotate(0, m_DegreesTurned, 0);

	if (m_SceneContext.pInput->IsActionTriggered(Inputs::reset))
	{
		LoadScene();
	}
}

void SpikeTestScene::OnGUI()
{
	ImGui::PushFont(ImguiFonts::pFont_DIN_Black_18);
	ImGui::Text("Rotation");
	ImGui::PopFont();

	const float minRotSpeedY{ 0.f };
	const float maxRotSpeedY{ 360.f };
	ImGui::SliderFloat("Rotation speed Y", &m_RotationSpeedY, minRotSpeedY, maxRotSpeedY);

	ImGui::PushFont(ImguiFonts::pFont_DIN_Black_18);
	ImGui::Text("Materials");
	ImGui::PopFont();

	m_pSpikeMat->DrawImGui();

}

void SpikeTestScene::OnSceneActivated()
{
	LoadScene();
}

void SpikeTestScene::LoadScene()
{
	auto lightDir = XMFLOAT3{ 0.577f,0.577f,0.577f };
	m_SceneContext.pGameTime->Reset();
	m_DegreesTurned = 0.f;
	m_SpikeLength = 2.f;
	m_RotationSpeedY = 20.f;
	m_pSpikeMat->SetVariable_Vector(L"gLightDirection", lightDir );
	m_pSpikeMat->SetSpikeLength(m_SpikeLength);
	auto pGameObject = m_SceneContext.pCamera->GetGameObject();
	if (!pGameObject)
	{
		Logger::LogError(L"Camera GameObject not found!!");
		return;
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
	pTransform->Translate(0, 50, -80);
}
