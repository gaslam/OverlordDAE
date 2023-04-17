#pragma once
class MeshFilter;
class ThirdPersonCamera final : public GameObject
{
public:
	ThirdPersonCamera(TransformComponent* pComponent, float distance = 5.f);
	~ThirdPersonCamera() override = default;
	ThirdPersonCamera(const ThirdPersonCamera& other) = delete;
	ThirdPersonCamera(ThirdPersonCamera&& other) noexcept = delete;
	ThirdPersonCamera& operator=(const ThirdPersonCamera& other) = delete;
	ThirdPersonCamera& operator=(ThirdPersonCamera&& other) noexcept = delete;

	void SetCameraDistance(float distance) { m_Distance = distance; }
	float GetCameraDistance() const { return m_Distance; }
	float GetPitch() const { return m_TotalPitch; }
	float GetYaw() const { return m_TotalYaw; }

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;
	void Intersect(MeshFilter* pMeshFilter);
private:
	float m_Distance{};
	float m_TotalPitch{}, m_TotalYaw{};
	const float m_MinAngle{ 5.f }, m_MaxAngle{ 85.f };
	float m_RotSpeed{15.f};
	TransformComponent* m_pComponent{ nullptr };
	CameraComponent* m_pCamera{ nullptr };
};

