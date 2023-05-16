#pragma once
class CoinObject :
    public GameObject
{
public:
	CoinObject(const XMFLOAT3& spawnPos,const float scale);
	CoinObject(const CoinObject& other) = delete;
	CoinObject(CoinObject&& other) noexcept = delete;
	CoinObject& operator=(const CoinObject& other) = delete;
	CoinObject& operator=(CoinObject&& other) noexcept = delete;
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

