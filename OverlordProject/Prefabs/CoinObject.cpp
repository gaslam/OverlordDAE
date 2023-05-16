#include "stdafx.h"
#include "CoinObject.h"

#include "Character.h"
#include "ProjectUtils.h"
#include "Materials/ColorMaterial.h"


CoinObject::CoinObject(const XMFLOAT3& spawnPos,const float scale) : m_Pos(spawnPos), m_Scale(scale)
{
}

void CoinObject::Initialize(const SceneContext&)
{
	auto* modelComponentBody = new ModelComponent{ L"Meshes/Objects/Coin.ovm" };
	AddComponent<ModelComponent>(modelComponentBody);

	auto transform = modelComponentBody->GetTransform();
	transform->Translate(m_Pos);

	ColorMaterial* pColorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	const XMFLOAT4 goldColor{ 0.807843137254902f,0.6745098039215686f,0.0509803921568627f,1.f };
	pColorMat->SetColor(goldColor);
	modelComponentBody->SetMaterial(pColorMat);
	transform->Scale(m_Scale);


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

void CoinObject::Update(const SceneContext& sceneContext)
{
	const float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	m_TotalYaw += elapsedTime * m_RotationSpeed;

	auto pTransform = GetTransform();
	pTransform->Rotate(0.f, m_TotalYaw, 0.f);
}