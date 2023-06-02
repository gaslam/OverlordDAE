#include "stdafx.h"
#include "CoinObject.h"

#include "Character.h"
#include "ProjectUtils.h"
#include "Materials/ColorMaterial.h"


CoinObject::CoinObject(const XMFLOAT3& spawnPos,const float scale) : m_Pos(spawnPos), m_Scale(scale)
{
}

void CoinObject::MarkForDeletion()
{
	m_MarkForDeletion = true;
	 auto pRigidComponent = GetComponent<RigidBodyComponent>();
	auto colliderInfo = pRigidComponent->GetCollider(0);
	colliderInfo.SetTrigger(false);
}

void CoinObject::Initialize(const SceneContext&)
{
	m_pModelComponentBody = new ModelComponent{ L"Meshes/Objects/Coin.ovm" };
	AddComponent<ModelComponent>(m_pModelComponentBody);

	auto transform = m_pModelComponentBody->GetTransform();
	transform->Translate(m_Pos);

	ColorMaterial* pColorMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	const XMFLOAT4 goldColor{ 0.807843137254902f,0.6745098039215686f,0.0509803921568627f,1.f };
	pColorMat->SetColor(goldColor);
	m_pModelComponentBody->SetMaterial(pColorMat);
	transform->Scale(m_Scale);


	auto& physx = PxGetPhysics();
	auto pPxMaterial = physx.createMaterial(1.f, 1.f, 1.f);

	auto pRigidBody = AddComponent(new RigidBodyComponent(true));
	int colliderId = pRigidBody->AddCollider(PxSphereGeometry{ m_Scale * 2.f }, *pPxMaterial);
	ColliderInfo colliderInfo = pRigidBody->GetCollider(colliderId);
	colliderInfo.SetTrigger(true);
	const auto particleObject = AddChild(new GameObject{});
	m_pParticleComponent = particleObject->AddComponent(new ParticleComponent{ L"Textures/Coin_Particle.png" });

	const auto pSoundManager{ SoundManager::Get() };
	auto pSoundSystem{ pSoundManager->GetSystem() };

	const char* soundData{ "Resources/Sounds/Collectibles/sm64_coin.wav" };
	FMOD_RESULT result = pSoundSystem->createStream(soundData, FMOD_DEFAULT, nullptr, &m_pSound);
	pSoundManager->ErrorCheck(result);

	SetOnTriggerCallBack([this,colliderInfo](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{

			if (dynamic_cast<Character*>(pOther) && pTrigger == this && action == PxTriggerAction::ENTER)
			{
				ActivateParticle();
			}
		});
}

void CoinObject::Update(const SceneContext& sceneContext)
{
	const float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	m_TotalYaw += elapsedTime * m_RotationSpeed;

	auto pTransform = GetTransform();
	pTransform->Rotate(0.f, m_TotalYaw, 0.f);
	if(m_IsWaitingForDeletion)
	{
		WaitAndDelete(elapsedTime);
	}

	auto playerSoundPos = ModelUtils::ToFmod(GetTransform()->GetPosition());
	auto soundVel = FMOD_VECTOR{0,0,0};
	m_pChannel3D->set3DAttributes(&playerSoundPos, &soundVel);
}

void CoinObject::WaitAndDelete(float elapsedTime)
{
	m_AccumulatedTime += elapsedTime;
	if (m_AccumulatedTime >= m_TimeToWaitForDeletion)
	{
		m_MarkForDeletion = true;
	}
}

void CoinObject::ActivateParticle()
{
	m_pParticleComponent->SetIsActive(true);

	m_pModelComponentBody->SetCanDraw(false);
	m_IsWaitingForDeletion = true;

	const auto pSoundManager{ SoundManager::Get() };
	auto pSoundSystem{ pSoundManager->GetSystem() };
	pSoundSystem->playSound(m_pSound, nullptr, false, &m_pChannel3D);
}
