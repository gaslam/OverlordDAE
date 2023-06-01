#include "stdafx.h"
#include "ParticleComponent.h"

void ParticleComponent::Initialize(const SceneContext&)
{
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,2.f,0.f };
	settings.minSize = 0.f;
	settings.maxSize = 2.f;
	settings.minEnergy = 1.f;
	settings.maxEnergy = 2.f;
	settings.minScale = 1.f;
	settings.maxScale = 1.f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .2f;
	settings.color = { 1.f,1.f,1.f, .6f };

	GameObject* gameObject{ GetGameObject() };
	m_pEmiter = gameObject->AddComponent(new ParticleEmitterComponent(L"Textures/Star_Particle.png", settings, 21));
}

void ParticleComponent::Update(const SceneContext& sceneContext)
{
	if(!m_IsActive)
	{
		return;
	}
	const float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	m_AccumulatedTime += elapsedTime;
	if(m_AccumulatedTime < m_TimeToBeActive)
	{
		return;
	}

	if(m_pEmiter->IsDisabled())
	{
		m_pEmiter->SetDisabled(false);
	}
	if (m_AccumulatedTime >= m_TimeBeforeDeletion)
	{
		m_IsActive = false;
		m_CanBeDeleted = true;
	}
}

void ParticleComponent::DrawImGUI() const
{
	if (m_pEmiter)
	{
		m_pEmiter->DrawImGui();
	}
}

void ParticleComponent::Draw(const SceneContext&)
{
}
