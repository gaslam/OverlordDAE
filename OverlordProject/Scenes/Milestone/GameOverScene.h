#pragma once
class ButtonComponent;
class GameOverScene :
    public GameScene
{
public:
	GameOverScene() : GameScene(L"GameOverScene") {}
	~GameOverScene() override = default;
	GameOverScene(const GameOverScene& other) = delete;
	GameOverScene(GameOverScene&& other) noexcept = delete;
	GameOverScene& operator=(const GameOverScene& other) = delete;
	GameOverScene& operator=(GameOverScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pBackgroundMusic{ nullptr };
	SpriteComponent* m_pSprite{};
	bool m_AutoRotate{ false };
	float m_TotalRotation{};

	const std::wstring m_MainMenuName{ L"EndMenu" };

	std::unordered_map<std::string, std::vector<ButtonComponent*>> m_MenuButtons{};
};

