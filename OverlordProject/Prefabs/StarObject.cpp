#include "stdafx.h"
#include "StarObject.h"

#include "Prefabs/Character.h"
#include "ProjectUtils.h"

StarObject::StarObject(const XMFLOAT3& spawnPos,const float scale) : m_Pos(spawnPos), m_Scale(scale)
{
}

void StarObject::Initialize(const SceneContext&)
{
	auto* modelComponentBody = new ModelComponent{ L"Meshes/Objects/Star/Star_Body.ovm" };
	AddComponent<ModelComponent>(modelComponentBody);

	auto transform = modelComponentBody->GetTransform();
	transform->Translate(m_Pos);

	ModelUtils::AddTextureToModelComponent(modelComponentBody, L"Textures/Objects/Star/Body/textureData.json");

	transform->Scale(m_Scale);

	GameObject* eyes = new GameObject{};
	AddChild(eyes);

	auto* modelComponentEyes = new ModelComponent{ L"Meshes/Objects/Star/Star_Eyes.ovm" };

	eyes->AddComponent<ModelComponent>(modelComponentEyes);

	ModelUtils::AddTextureToModelComponent(modelComponentEyes, L"Textures/Objects/Star/Eye/textureData.json");


	auto& physx = PxGetPhysics();
	auto pPxMaterial = physx.createMaterial(1.f, 1.f, 1.f);

	auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	int colliderId = pRigidBody->AddCollider(PxSphereGeometry{ m_Scale * 2.f }, *pPxMaterial);
	auto colliderInfo = pRigidBody->GetCollider(colliderId);
	colliderInfo.SetTrigger(true);

	SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (dynamic_cast<Character*>(pOther) && pTrigger == this && action == PxTriggerAction::ENTER)
			{
				m_MarkForDeletion = true;
			}
		});
}

void StarObject::Update(const SceneContext& sceneContext)
{
	const float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	m_TotalYaw += elapsedTime * m_RotationSpeed;

	auto pTransform = GetTransform();
	pTransform->Rotate(0.f, m_TotalYaw, 0.f);
}
