#pragma once
#include <vector>
class BoneObject;
class SoftwareSkinningScene_2 :
    public GameScene
{
public:
	SoftwareSkinningScene_2();
	~SoftwareSkinningScene_2() override = default;

	SoftwareSkinningScene_2(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&& other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;
	void OnSceneDeactivated() override;

private:
	std::vector<BoneObject*> m_pBones{};

	bool m_AutoRotate{ false };
	float m_BoneRotation{};
	std::vector<XMFLOAT4> m_BoneRotations{};
	float m_BoneRotationSpeedDeg{ 10.f };
	int m_RotationSign{ 1 };

	void RotateBones();

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color):
		transformedVertex{position,normal,color},
		originalVertex{position,normal,color}
		{}

		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};

