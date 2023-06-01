#include "stdafx.h"
#include "StarObject.h"

#include "Prefabs/Character.h"
#include "ProjectUtils.h"
#include "Components/NumberDisplayComponent.h"
#include "Components/ParticleComponent.h"

StarObject::StarObject(const XMFLOAT3& spawnPos,const float scale) : m_Pos(spawnPos), m_Scale(scale)
{
}

void StarObject::ActivateParticle()
{
 	m_pParticleComponent->SetIsActive(true);
	if (m_pParticleComponent->GetIsActive())
	{
		for (auto child : GetChildren<GameObject>())
		{
			if (auto pComponent{ child->GetComponent<ModelComponent>(true) })
			{
				pComponent->SetCanDraw(false);
			}
		}
	}
}

void StarObject::DrawImGUI() const
{
	if(m_pParticleComponent)
	{
		m_pParticleComponent->DrawImGUI();
	}
}

void StarObject::Initialize(const SceneContext&)
{
	m_pStarObject = AddChild(new GameObject);
	auto* modelComponentBody = new ModelComponent{ L"Meshes/Objects/Star/Star_Body.ovm" };
	m_pStarObject->AddComponent<ModelComponent>(modelComponentBody);

	auto transform = m_pStarObject->GetTransform();
	transform->Translate(m_Pos);

	ModelUtils::AddTextureToModelComponent(modelComponentBody, L"Textures/Objects/Star/Body/textureData.json");

	transform->Scale(m_Scale);

	GameObject* eyes = new GameObject{};
	m_pStarObject->AddChild(eyes);

	auto* modelComponentEyes = new ModelComponent{ L"Meshes/Objects/Star/Star_Eyes.ovm" };

	eyes->AddComponent<ModelComponent>(modelComponentEyes);

	ModelUtils::AddTextureToModelComponent(modelComponentEyes, L"Textures/Objects/Star/Eye/textureData.json");


	auto& physx = PxGetPhysics();
	auto pPxMaterial = physx.createMaterial(1.f, 1.f, 1.f);

	auto pRigidBody = m_pStarObject->AddComponent(new RigidBodyComponent(true));
	int colliderId = pRigidBody->AddCollider(PxSphereGeometry{ m_Scale * 2.f }, *pPxMaterial);
	auto colliderInfo = pRigidBody->GetCollider(colliderId);
	colliderInfo.SetTrigger(true);
	const auto pObject2 = AddChild(new GameObject);
	m_pParticleComponent = pObject2->AddComponent(new ParticleComponent{});
	transform = pObject2->GetTransform();
	transform->Translate(m_Pos);
	m_pStarObject->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (dynamic_cast<Character*>(pOther) && pTrigger == m_pStarObject && action == PxTriggerAction::ENTER)
			{
				ActivateParticle();
			}
		});

}

void StarObject::Update(const SceneContext& sceneContext)
{
	const float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	m_TotalYaw += elapsedTime * m_RotationSpeed;

	auto pTransform = m_pStarObject->GetTransform();
	pTransform->Rotate(0.f, m_TotalYaw, 0.f);
}
