#pragma once
class FontTestScene : public GameScene
{
public:
	FontTestScene() :GameScene(L"FontTestScene") {}
	~FontTestScene() override = default;
	FontTestScene(const FontTestScene& other) = delete;
	FontTestScene(FontTestScene&& other) noexcept = delete;
	FontTestScene& operator=(const FontTestScene& other) = delete;
	FontTestScene& operator=(FontTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;
	void OnSceneActivated() override;

private:
	SpriteFont* m_pFont{};
	SpriteFont* m_pFont2{};

	std::string m_Text{  };
	XMFLOAT2 m_TextPosition{};
	XMFLOAT4 m_TextColor{ };

	void LoadScene();

	enum InputIds : int {
		reset
	};
};