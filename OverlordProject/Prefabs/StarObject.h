#pragma once
class ParticleComponent;

class StarObject :
    public GameObject
{
public:
	StarObject(const XMFLOAT3& spawnPos,const float scale);
	~StarObject() override = default;
	StarObject(const StarObject& other) = delete;
	StarObject(StarObject&& other) noexcept = delete;
	StarObject& operator=(const StarObject& other) = delete;
	StarObject& operator=(StarObject&& other) noexcept = delete;
	bool IsMarkedAsDeleted() const { return m_MarkForDeletion; }
	bool IsWaitingForDeletion() const { return m_IsWaitingForDeletion; }
	void DrawImGUI() const;
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;
private:
	bool m_MarkForDeletion{ false };
	bool m_IsWaitingForDeletion{ false };
	bool m_IsMarkedAsFinal{ true };
	float m_Scale{ 1.f };
	float m_RotationSpeed{ 100.f };
	float m_TotalYaw{};
	float m_TimeToWaitForDeletion{ 1.5f };
	float m_AccumulatedTime{ 0.f };

	XMFLOAT3 m_Pos{};

	GameObject* m_pStarObject{};

	ParticleComponent* m_pParticleComponent{};
	ModelComponent* m_pModelComponentBody{};
	ModelComponent* m_pModelComponentEyes{};
	FMOD::Channel* m_pChannel3D{};
	FMOD::Sound* m_pSound{};

	void WaitAndDelete(float elapsedTime);
	void ActivateParticle();
};

