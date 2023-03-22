#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include <Materials/ColorMaterial.h>
#include <Prefabs/BoneObject.h>

SoftwareSkinningScene_1::SoftwareSkinningScene_1() : GameScene(L"SoftwareSkinningScene_1")
{
}

void SoftwareSkinningScene_1::Initialize()
{
	auto colorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	auto pRoot = new GameObject();
	const float boneLength{ 15.f };
	m_pBone0 = new BoneObject{colorMat,boneLength};
	m_pBone1 = new BoneObject{ colorMat,boneLength };

	AddChild(pRoot);
	pRoot->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);
	m_SceneContext.settings.enableOnGUI = true;
}

void SoftwareSkinningScene_1::Update()
{
	RotateBones();
}

void SoftwareSkinningScene_1::OnGUI()
{
	auto pBone0Transform = m_pBone0->GetTransform();
	auto pBone1Transform = m_pBone1->GetTransform();

	ImGui::Checkbox("auto rotate", &m_AutoRotate);
		ImGui::DragFloat3("Bone 0 - ROT", ConvertUtil::ToImFloatPtr(m_Bone0Rotation),1.f);
	pBone0Transform->Rotate(m_Bone0Rotation.x,m_Bone0Rotation.y,m_Bone0Rotation.z);

	ImGui::DragFloat3("Bone 1 - ROT", ConvertUtil::ToImFloatPtr(m_Bone1Rotation), 1.f);
	pBone1Transform->Rotate(m_Bone1Rotation.x, m_Bone1Rotation.y, m_Bone1Rotation.z);
}

void SoftwareSkinningScene_1::RotateBones()
{
	if (!m_AutoRotate)
	{
		return;
	}
	const float minRotDeg{ -45.f }, maxRotDeg{ 45.f };

	auto pGameTime = m_SceneContext.pGameTime;
	float degToAdd{};

	if (m_RotationSign == 1 && m_BoneRotation > maxRotDeg)
	{
		m_RotationSign = -m_RotationSign;
	}

	if (m_RotationSign == -1 && m_BoneRotation < minRotDeg)
	{
		m_RotationSign = -m_RotationSign;
	}

	if (pGameTime)
	{
		degToAdd = pGameTime->GetElapsed() * m_RotationSign * m_BoneRotationSpeedDeg;
		m_BoneRotation += degToAdd;
	}
	m_Bone0Rotation.z += degToAdd;

	m_Bone1Rotation.z += -degToAdd * 2.f;
}
