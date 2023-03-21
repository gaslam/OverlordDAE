#pragma once
class SpikeMaterial;
class SpikeTestScene :
    public GameScene
{
public:
	SpikeTestScene() :GameScene(L"SpikeTestScene") {}
	~SpikeTestScene() override = default;

	SpikeTestScene(const SpikeTestScene& other) = delete;
	SpikeTestScene(SpikeTestScene&& other) noexcept = delete;
	SpikeTestScene& operator=(const SpikeTestScene& other) = delete;
	SpikeTestScene& operator=(SpikeTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;
private:
	SpikeMaterial* m_pSpikeMat{ nullptr };
	float m_RotationSpeedY{ 20.f };
	float m_SpikeLength{ 2.f };
	float m_DegreesTurned{};
	GameObject* m_pSphere{ nullptr };
	XMFLOAT4 m_Color{ XMFLOAT4{Colors::Red} };
};

