#pragma once
class ButtonComponent;
class EndMenuScene :
    public GameScene
{
public:
	EndMenuScene() : GameScene(L"EndMenuScene") {}
	~EndMenuScene() override = default;
	EndMenuScene(const EndMenuScene& other) = delete;
	EndMenuScene(EndMenuScene&& other) noexcept = delete;
	EndMenuScene& operator=(const EndMenuScene& other) = delete;
	EndMenuScene& operator=(EndMenuScene&& other) noexcept = delete;

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

