#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length) :
	m_pMaterial{pMaterial}, m_Length{length}
{
}

void BoneObject::AddBone(BoneObject* pObject)
{
	auto pTransform = pObject->GetTransform();

	if (pTransform)
	{
		pTransform->Translate(m_Length, 0, 0);
	}
	AddChild(pObject);
}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty = new GameObject{};
	AddChild(pEmpty);
	ModelComponent* pComponent = new ModelComponent{ L"Meshes/Bone.ovm" };
	pComponent->SetMaterial(m_pMaterial);
	pEmpty->AddComponent(pComponent);
	const float degreesToRotate{ -90.f };
	auto pTransform = pEmpty->GetTransform();
	pTransform->Rotate(0, degreesToRotate, 0);
	pTransform->Scale(m_Length,m_Length, m_Length);
}
