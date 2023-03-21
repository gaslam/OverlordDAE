#include "stdafx.h"
#include "SpikeTestScene.h"
#include "Materials/SpikeMaterial.h"

void SpikeTestScene::Initialize()
{
	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/OctaSphere.ovm"} };
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
}

void SpikeTestScene::Update()
{
	float deltaT = m_SceneContext.pGameTime->GetElapsed();
	m_DegreesTurned += deltaT * m_RotationSpeedY;
	m_pSphere->GetTransform()->Rotate(0, m_DegreesTurned, 0);
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
