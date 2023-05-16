#pragma once
class StarObject :
    public GameObject
{
public:
	StarObject(const XMFLOAT3& spawnPos,const float scale);
	StarObject(const StarObject& other) = delete;
	StarObject(StarObject&& other) noexcept = delete;
	StarObject& operator=(const StarObject& other) = delete;
	StarObject& operator=(StarObject&& other) noexcept = delete;
	bool IsMarkedAsDeleted() const { return m_MarkForDeletion; }
protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;
private:
	float m_Scale{ 1.f };
	XMFLOAT3 m_Pos{};
	bool m_MarkForDeletion{ false };

	float m_RotationSpeed{ 100.f };
	float m_TotalYaw{};
};

