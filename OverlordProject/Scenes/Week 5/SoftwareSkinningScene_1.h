#pragma once

class BoneObject;
class SoftwareSkinningScene_1 :
    public GameScene
{
public:
	SoftwareSkinningScene_1();
	~SoftwareSkinningScene_1() override = default;

	SoftwareSkinningScene_1(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	BoneObject* m_pBone0{},* m_pBone1{};

	bool m_AutoRotate{ false };
	float m_BoneRotation{};
	XMFLOAT4 m_Bone0Rotation{};
	XMFLOAT4 m_Bone1Rotation{};
	float m_BoneRotationSpeedDeg{ 10.f };
	int m_RotationSign{1};

	void RotateBones();
};

