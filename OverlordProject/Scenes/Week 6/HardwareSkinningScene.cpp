#include "stdafx.h"
#include "HardwareSkinningScene.h"

#include "Materials/DiffuseMaterial_Skinned.h"

HardwareSkinningScene::~HardwareSkinningScene()
{
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		delete[] m_ClipNames[i];
	}

	delete[] m_ClipNames;
}

void HardwareSkinningScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	const auto pSkinnedMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pSkinnedMaterial->SetDiffuseTexture(L"Textures/PeasantGirl_Diffuse.png");

	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/Characters/PeasantGirl.ovm"));
	pModel->SetMaterial(pSkinnedMaterial);

	pObject->GetTransform()->Scale(0.15f);

	pAnimator = pModel->GetAnimator();
	pAnimator->SetAnimation(m_AnimationClipId);
	pAnimator->SetAnimationSpeed(m_AnimationSpeed);

	//Gather Clip Names
	m_ClipCount = pAnimator->GetClipCount();
	m_ClipNames = new char* [m_ClipCount];
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		auto clipName = StringUtil::utf8_encode(pAnimator->GetClip(static_cast<int>(i)).name);
		const auto clipSize = clipName.size();
		m_ClipNames[i] = new char[clipSize + 1];
		strncpy_s(m_ClipNames[i], clipSize + 1, clipName.c_str(), clipSize);
	}

	m_SceneContext.pInput->AddInputAction(InputAction{ Inputs::reset,InputState::pressed,'R' });
}

void HardwareSkinningScene::OnGUI()
{
	if (ImGui::Button(pAnimator->IsPlaying() ? "PAUSE" : "PLAY"))
	{
		if (pAnimator->IsPlaying())pAnimator->Pause();
		else pAnimator->Play();
	}

	if (ImGui::Button("RESET"))
	{
		pAnimator->Reset();
	}

	ImGui::Dummy({ 0,5 });

	bool reversed = pAnimator->IsReversed();
	if (ImGui::Checkbox("Play Reversed", &reversed))
	{
		pAnimator->SetPlayReversed(reversed);
	}

	if (ImGui::ListBox("Animation Clip", &m_AnimationClipId, m_ClipNames, static_cast<int>(m_ClipCount)))
	{
		pAnimator->SetAnimation(m_AnimationClipId);
	}

	if (ImGui::SliderFloat("Animation Speed", &m_AnimationSpeed, 0.f, 4.f))
	{
		pAnimator->SetAnimationSpeed(m_AnimationSpeed);
	}
}

void HardwareSkinningScene::OnSceneActivated()
{
	LoadScene();
}

void HardwareSkinningScene::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(Inputs::reset))
	{
		LoadScene();
	}
}

void HardwareSkinningScene::LoadScene()
{
	pAnimator->Reset();
	pAnimator->Play();
	pAnimator->SetAnimation(0);
	if (pAnimator->IsReversed())
	{
		pAnimator->SetPlayReversed(false);
	}

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
