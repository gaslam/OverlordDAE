#pragma once
class StaticCamera final :
    public GameObject
{
public:
	StaticCamera() = default;
	~StaticCamera() override = default;
	StaticCamera(const StaticCamera& other) = delete;
	StaticCamera(StaticCamera&& other) noexcept = delete;
	StaticCamera& operator=(const StaticCamera& other) = delete;
	StaticCamera& operator=(StaticCamera&& other) noexcept = delete;

protected:

	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext&) override {};
private:
	CameraComponent* m_pCamera{};
};

