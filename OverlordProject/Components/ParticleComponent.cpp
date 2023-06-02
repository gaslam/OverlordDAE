#include "stdafx.h"
#include "ParticleComponent.h"

void ParticleComponent::Initialize(const SceneContext&)
{
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,2.f,0.f };
	settings.minSize = 0.f;
	settings.maxSize = .5f;
	settings.minEnergy = 0.f;
	settings.maxEnergy = 0.3f;
	settings.minScale = 0.f;
	settings.maxScale = 1.f;
	settings.minEmitterRadius = 0.f;
	settings.maxEmitterRadius = .2f;
	settings.color = { 1.f,1.f,1.f, .6f };

	GameObject* gameObject{ GetGameObject() };
	m_pEmiter = gameObject->AddComponent(new ParticleEmitterComponent(m_AssetFile, settings, 21));
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

	if(!m_pEmiter->IsDisabled())
	{
		m_pEmiter->SetDisabled(true);
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
