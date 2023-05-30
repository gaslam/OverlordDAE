#pragma once
class BallPrefab :
    public GameObject
{
public:
	BallPrefab(const XMFLOAT4& color, const float scale);
	~BallPrefab() override = default;
	BallPrefab(const BallPrefab& other) = delete;
	BallPrefab(BallPrefab&& other) noexcept = delete;
	BallPrefab& operator=(const BallPrefab& other) = delete;
	BallPrefab& operator=(BallPrefab&& other) noexcept = delete;
	bool IsMarkedAsDeleted() const { return m_MarkForDeletion; }
	bool MustBePushed() const { return m_MustBePushed; }
	XMFLOAT3 GetDirToBePushed() const { return m_DirToBePushed; }

	void SetMarkForDeletion(bool markForDeletion) { m_MarkForDeletion = markForDeletion; }
	void SetMustBePushed(bool canPush) { m_MustBePushed = canPush; }
	void SetDirToBePushed(XMFLOAT3& dir) { m_DirToBePushed = dir; }
protected:
	void Initialize(const SceneContext&) override;
private:
	bool m_MarkForDeletion{false};
	bool m_MustBePushed{ false };
	const float m_Scale{};
	XMFLOAT3 m_DirToBePushed{};
	XMFLOAT4 m_Color;
};

