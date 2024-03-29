#pragma once
class SpherePrefab;
class UberMaterialScene :
    public GameScene
{
public:
	UberMaterialScene();
	~UberMaterialScene() override = default;

	UberMaterialScene(const UberMaterialScene& other) = delete;
	UberMaterialScene(UberMaterialScene&& other) noexcept = delete;
	UberMaterialScene& operator=(const UberMaterialScene& other) = delete;
	UberMaterialScene& operator=(UberMaterialScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void LoadScene();
	void OnSceneActivated() override;

private:
	GameObject* m_pSphere{ nullptr };
	bool m_MaterialPopupWindowOpwn{ false };
	int m_SelectedTechnique{};

	UINT m_UberMatId{};
	enum Inputs : int {
		reset
	};
};

