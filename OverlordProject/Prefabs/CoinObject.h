#pragma once
#include "Components/ParticleComponent.h"

class CoinObject :
    public GameObject
{
public:
	CoinObject(const XMFLOAT3& spawnPos,const float scale);
	~CoinObject() override = default;
	CoinObject(const CoinObject& other) = delete;
	CoinObject(CoinObject&& other) noexcept = delete;
	CoinObject& operator=(const CoinObject& other) = delete;
	CoinObject& operator=(CoinObject&& other) noexcept = delete;
	bool IsMarkedAsDeleted() const { return m_MarkForDeletion; }
	bool IsWaitingForDeletion() const { return m_IsWaitingForDeletion; }
	void MarkForDeletion();
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;
private:
	float m_Scale{ 1.f };
	XMFLOAT3 m_Pos{};
	bool m_MarkForDeletion{ false };
	bool m_IsWaitingForDeletion{ false };
	bool m_IsMarkedAsFinal{ true };
	float m_TimeToWaitForDeletion{ 1.5f };
	float m_AccumulatedTime{ 0.f };

	float m_RotationSpeed{ 100.f };
	float m_TotalYaw{};
	ParticleComponent* m_pParticleComponent{};
	ModelComponent* m_pModelComponentBody{};
	FMOD::Channel* m_pChannel3D{};
	FMOD::Sound* m_pSound{};
	void WaitAndDelete(float elapsedTime);
	void ActivateParticle();
};

