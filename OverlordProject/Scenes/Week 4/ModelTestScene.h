#pragma once
class ModelTestScene :
    public GameScene
{
public:
	ModelTestScene();
	~ModelTestScene() override = default;

	ModelTestScene(const ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void LoadScene();
	void OnSceneActivated() override;

private:
	GameObject* m_pChair{nullptr};
	enum Inputs : int {
		reset
	};
};

