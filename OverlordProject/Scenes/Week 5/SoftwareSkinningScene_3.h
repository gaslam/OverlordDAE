#pragma once
#include <vector>
class BoneObject;
class SoftwareSkinningScene_3 :
    public GameScene
{
public:
	SoftwareSkinningScene_3();
	~SoftwareSkinningScene_3() override = default;

	SoftwareSkinningScene_3(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;
	void OnSceneActivated() override;

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
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float blendWeight) :
			transformedVertex{ position,normal,color },
			originalVertex{ position,normal,color },
			blendWeight0{blendWeight},
			blendWeight1{1.f - blendWeight}
		{}

		float blendWeight0{};
		float blendWeight1{};
		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
	};
	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};

	void LoadScene();
	enum Inputs : int {
		reset
	};
};

