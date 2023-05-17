#pragma once
#include <unordered_map>

class ButtonComponent;
class MainMenuScene :
    public GameScene
{
public:
	MainMenuScene() : GameScene(L"MainMenuScene") {}
	~MainMenuScene() override = default;
	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pBackgroundMusic{ nullptr };
	bool m_AutoRotate{ false };
	float m_TotalRotation{};

	const std::wstring m_MainMenuName{ L"MainMenu" };
	const std::wstring m_ControlMenuName{ L"ControlMenu" };

	void SetMenu(const std::string& name);

	std::unordered_map<std::string, std::vector<ButtonComponent*>> m_MenuButtons{};
	std::unordered_map<std::string, SpriteComponent*> m_pSprites{};
};

