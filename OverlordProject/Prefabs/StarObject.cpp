#include "stdafx.h"
#include "StarObject.h"

#include "Prefabs/Character.h"
#include "ProjectUtils.h"
#include "Components/ParticleComponent.h"

StarObject::StarObject(const XMFLOAT3& spawnPos, const float scale) : m_Pos(spawnPos), m_Scale(scale)
{
}

void StarObject::ActivateParticle()
{
	m_pParticleComponent->SetIsActive(true);

	m_pModelComponentBody->SetCanDraw(false);
	m_pModelComponentEyes->SetCanDraw(false);
	m_IsWaitingForDeletion = true;

	const auto pSoundManager{ SoundManager::Get() };
	auto pSoundSystem{ pSoundManager->GetSystem() };
	pSoundSystem->playSound(m_pSound, nullptr, false, &m_pChannel3D);

	if(m_IsMarkedAsFinal)
	{
		auto pScene{ GetScene() };
		pScene->End();
	}
}

void StarObject::DrawImGUI() const
{
	if (m_pParticleComponent)
	{
		m_pParticleComponent->DrawImGUI();
	}
}

void StarObject::Initialize(const SceneContext&)
{
	m_pStarObject = AddChild(new GameObject);
	m_pModelComponentBody = new ModelComponent{ L"Meshes/Objects/Star/Star_Body.ovm" };
	m_pStarObject->AddComponent<ModelComponent>(m_pModelComponentBody);

	auto transform = m_pStarObject->GetTransform();
	transform->Translate(m_Pos);

	ModelUtils::AddTextureToModelComponent(m_pModelComponentBody, L"Textures/Objects/Star/Body/textureData.json");

	transform->Scale(m_Scale);

	GameObject* eyes = new GameObject{};
	m_pStarObject->AddChild(eyes);

	m_pModelComponentEyes = new ModelComponent{ L"Meshes/Objects/Star/Star_Eyes.ovm" };

	eyes->AddComponent<ModelComponent>(m_pModelComponentEyes);

	ModelUtils::AddTextureToModelComponent(m_pModelComponentEyes, L"Textures/Objects/Star/Eye/textureData.json");


	auto& physx = PxGetPhysics();
	auto pPxMaterial = physx.createMaterial(1.f, 1.f, 1.f);

	auto pRigidBody = m_pStarObject->AddComponent(new RigidBodyComponent(true));
	int colliderId = pRigidBody->AddCollider(PxSphereGeometry{ m_Scale * 2.f }, *pPxMaterial);
	auto colliderInfo = pRigidBody->GetCollider(colliderId);
	colliderInfo.SetTrigger(true);
	const auto pObject2 = AddChild(new GameObject{});
	m_pParticleComponent = pObject2->AddComponent(new ParticleComponent{ L"Textures/Star_Particle.png" });
	transform = pObject2->GetTransform();
	transform->Translate(m_Pos);
	m_pStarObject->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (dynamic_cast<Character*>(pOther) && pTrigger == m_pStarObject && action == PxTriggerAction::ENTER)
			{
				ActivateParticle();
			}
		});
	const auto pSoundManager{ SoundManager::Get() };
	auto pSoundSystem{ pSoundManager->GetSystem() };

	const char* soundData{ "Resources/Sounds/Collectibles/sm64_coin.wav" };
	FMOD_RESULT result = pSoundSystem->createStream(soundData, FMOD_DEFAULT, nullptr, &m_pSound);
	pSoundManager->ErrorCheck(result);
}

void StarObject::Update(const SceneContext& sceneContext)
{
	const float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	m_TotalYaw += elapsedTime * m_RotationSpeed;

	auto pTransform = m_pStarObject->GetTransform();
	pTransform->Rotate(0.f, m_TotalYaw, 0.f);
	if (m_IsWaitingForDeletion)
	{
		WaitAndDelete(elapsedTime);
	}
}

void StarObject::WaitAndDelete(float elapsedTime)
{
	m_AccumulatedTime += elapsedTime;
	if(m_AccumulatedTime >= m_TimeToWaitForDeletion)
	{
		m_MarkForDeletion = true;
	}
}
