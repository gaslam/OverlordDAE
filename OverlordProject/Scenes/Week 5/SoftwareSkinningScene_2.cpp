#include "stdafx.h"
#include "SoftwareSkinningScene_2.h"
#include "Prefabs/BoneObject.h"
#include "Materials/ColorMaterial.h"
#include <algorithm>
SoftwareSkinningScene_2::SoftwareSkinningScene_2() : GameScene(L"SoftwareSkinningScene_2")
{
}

void SoftwareSkinningScene_2::Initialize()
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
}

void SoftwareSkinningScene_2::Update()
{
	RotateBones();
	size_t bone0verts{ 24 };
	auto pTransform = m_pBones[0]->GetTransform();
	if (pTransform)
	{
		XMMATRIX bindPosMatrix = XMLoadFloat4x4(&m_pBones[0]->GetBindPos());
		XMMATRIX worldMatrix = XMLoadFloat4x4(&pTransform->GetWorld());
		auto boneTransform = bindPosMatrix * worldMatrix;

		for (size_t i{}; i < bone0verts; ++i)
		{
			auto pos = m_SkinnedVertices[i].originalVertex.Position;
			auto vector = XMLoadFloat3(&pos);
			auto transformedVector = XMVector3TransformCoord(vector, boneTransform);
			XMStoreFloat3(&pos,transformedVector);
			m_SkinnedVertices[i].transformedVertex.Position = pos;
		}
	}

	pTransform = m_pBones[1]->GetTransform();
	if (pTransform)
	{
		XMMATRIX bindPosMatrix = XMLoadFloat4x4(&m_pBones[1]->GetBindPos());
		XMMATRIX worldMatrix = XMLoadFloat4x4(&pTransform->GetWorld());
		auto boneTransform = bindPosMatrix * worldMatrix;

		for (size_t i{bone0verts}; i < m_SkinnedVertices.size(); ++i)
		{
			auto pos = m_SkinnedVertices[i].originalVertex.Position;
			auto vector = XMLoadFloat3(&pos);
			auto transformedVector = XMVector3TransformCoord(vector, boneTransform);
			XMStoreFloat3(&pos, transformedVector);
			m_SkinnedVertices[i].transformedVertex.Position = pos;
		}
	}

	m_pMeshDrawer->RemoveTriangles();
	for (size_t i{}; i < m_SkinnedVertices.size(); i += 4)
	{
		const QuadPosNormCol quad{ m_SkinnedVertices[i].transformedVertex,m_SkinnedVertices[i + 1].transformedVertex
			,m_SkinnedVertices[i + 2].transformedVertex, m_SkinnedVertices[i + 3].transformedVertex };
		m_pMeshDrawer->AddQuad(quad);
	}
	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_2::OnGUI()
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

void SoftwareSkinningScene_2::OnSceneDeactivated()
{
	std::fill_n(m_BoneRotations.begin(),m_BoneRotations.size(), XMFLOAT4{});
	m_BoneRotation = 0.f;
}

void SoftwareSkinningScene_2::RotateBones()
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
	m_BoneRotations[0].z += degToAdd;

	m_BoneRotations[1].z += -degToAdd * 2.f;
}

void SoftwareSkinningScene_2::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion
}
