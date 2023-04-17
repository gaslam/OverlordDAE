#include "stdafx.h"
#include "SoftwareSkinningScene_3.h"
#include "Prefabs/BoneObject.h"
#include "Materials/ColorMaterial.h"
#include <algorithm>
SoftwareSkinningScene_3::SoftwareSkinningScene_3() : GameScene(L"SoftwareSkinningScene_3")
{
}

void SoftwareSkinningScene_3::Initialize()
{
	auto colorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	auto pRoot = new GameObject();
	const float boneLength{ 15.f };
	m_pBones.push_back(new BoneObject{ colorMat,boneLength });
	m_pBones.push_back(new BoneObject{ colorMat,boneLength });

	AddChild(pRoot);
	pRoot->AddChild(m_pBones[0]);
	m_pBones[0]->AddBone(m_pBones[1]);
	m_SceneContext.settings.enableOnGUI = true;
	m_pBones[0]->CalculateBindPos();
	m_BoneRotations.resize(m_pBones.size());
	const UINT triangleCapacity{ 24 };
	const bool enableTransparency{ true };
	auto pBoxDrawer = new GameObject{};
	m_pMeshDrawer = pBoxDrawer->AddComponent<MeshDrawComponent>(new MeshDrawComponent{ triangleCapacity,enableTransparency });
	AddChild(pBoxDrawer);
	InitializeVertices(boneLength);
	m_SceneContext.pInput->AddInputAction(InputAction{ Inputs::reset,InputState::pressed,'R' });
}

void SoftwareSkinningScene_3::Update()
{
	RotateBones();
	auto bone0Transform{ XMLoadFloat4x4(&m_pBones[0]->GetBindPos()) * XMLoadFloat4x4(&m_pBones[0]->GetTransform()->GetWorld())};
	auto bone1Transform{ XMLoadFloat4x4(&m_pBones[1]->GetBindPos()) * XMLoadFloat4x4(&m_pBones[1]->GetTransform()->GetWorld())};
	const size_t vertexCount{ m_SkinnedVertices.size() / 2};
	for (size_t i = 0; i < m_SkinnedVertices.size(); i++)
	{
		auto positionBone0{ XMVector3TransformCoord(XMLoadFloat3(&m_SkinnedVertices[i].originalVertex.Position), bone0Transform) };
		auto positionBone1{ XMVector3TransformCoord(XMLoadFloat3(&m_SkinnedVertices[i].originalVertex.Position), bone1Transform) };
		XMVECTOR lerp{  };
		if (i > vertexCount)
		{
			lerp = positionBone1 * m_SkinnedVertices[i].blendWeight0 + positionBone0 * m_SkinnedVertices[i].blendWeight1;
		}
		else
		{
			lerp = positionBone0 * m_SkinnedVertices[i].blendWeight0 + positionBone1 * m_SkinnedVertices[i].blendWeight1;
		}
		XMStoreFloat3(&m_SkinnedVertices[i].transformedVertex.Position, lerp);
	}

	m_pMeshDrawer->RemoveTriangles();
	for (size_t i{}; i < m_SkinnedVertices.size(); i += 4)
	{
		const QuadPosNormCol quad{ m_SkinnedVertices[i].transformedVertex,m_SkinnedVertices[i + 1].transformedVertex
			,m_SkinnedVertices[i + 2].transformedVertex, m_SkinnedVertices[i + 3].transformedVertex };
		m_pMeshDrawer->AddQuad(quad);
	}
	m_pMeshDrawer->UpdateBuffer();

	if (m_SceneContext.pInput->IsActionTriggered(Inputs::reset))
	{
		LoadScene();
	}
}

void SoftwareSkinningScene_3::OnGUI()
{
	ImGui::Checkbox("auto rotate", &m_AutoRotate);
	for (size_t i{}; i < m_pBones.size(); ++i)
	{
		std::string sliderLabel{ "Bone " + std::to_string(i) + " - ROT" };
		ImGui::DragFloat3(sliderLabel.c_str(), ConvertUtil::ToImFloatPtr(m_BoneRotations[i]), 1.f);
		auto pTransform = m_pBones[i]->GetTransform();
		if (pTransform)
		{
			pTransform->Rotate(m_BoneRotations[i].x, m_BoneRotations[i].y, m_BoneRotations[i].z);
		}
	}

}

void SoftwareSkinningScene_3::LoadScene()
{
	std::fill_n(m_BoneRotations.begin(), m_BoneRotations.size(), XMFLOAT4{});
	m_BoneRotation = 0.f;
	const auto pFreeCamera = m_SceneContext.pCamera;
	pFreeCamera->GetTransform()->Rotate(30, 0, 0, false);
	pFreeCamera->GetTransform()->Translate(0, 50, -80);

}

void SoftwareSkinningScene_3::OnSceneActivated()
{
	LoadScene();
}

void SoftwareSkinningScene_3::RotateBones()
{
	if (!m_AutoRotate)
	{
		m_BoneRotation = 0;
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
	m_BoneRotations[0].z += degToAdd;

	m_BoneRotations[1].z += -degToAdd * 2.f;
}

void SoftwareSkinningScene_3::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
	const float blendWeightFull{ 1.f };
	const float blendWeightHalf{ .5f };
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightHalf);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);

#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightHalf);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightHalf);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightHalf);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightHalf);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col, blendWeightFull);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col, blendWeightFull);
#pragma endregion
}