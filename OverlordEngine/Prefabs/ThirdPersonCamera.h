#pragma once
class MeshFilter;
class ThirdPersonCamera final : public GameObject
{
public:
	explicit ThirdPersonCamera(float distance = 5.f);
	~ThirdPersonCamera() override = default;
	ThirdPersonCamera(const ThirdPersonCamera& other) = delete;
	ThirdPersonCamera(ThirdPersonCamera&& other) noexcept = delete;
	ThirdPersonCamera& operator=(const ThirdPersonCamera& other) = delete;
	ThirdPersonCamera& operator=(ThirdPersonCamera&& other) noexcept = delete;

	void SetCameraDistance(float distance) { m_Distance = distance; }
	[[nodiscard]] float GetCameraDistance() const { return m_Distance; }
	[[nodiscard]] float GetPitch() const { return m_TotalPitch; }
	[[nodiscard]] float GetYaw() const { return m_TotalYaw; }
	void AddModelToAvoid(ModelComponent* modelComponent);
	void RemoveModelToAvoid(ModelComponent* modelComponent);

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext&) override;
	void Intersect(MeshFilter* pMeshFilter);
private:
	float m_Distance{};
	float m_TotalPitch{}, m_TotalYaw{};
	const float m_MinAngle{ 5.f }, m_MaxAngle{ 85.f };
	float m_MouseRotSpeed{10.f};
	float m_ControllerRotSpeed{50.f};
	XMFLOAT3 m_BackwardsDir{};
	TransformComponent* m_pComponent{ nullptr };
	CameraComponent* m_pCamera{ nullptr };
	std::vector<ModelComponent*> m_pModelsToAvoid{};
};

