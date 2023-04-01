#pragma once
class BanjoTestScene :
    public GameScene
{
public:
	BanjoTestScene() : GameScene(L"BanjoTestScene") {}
	~BanjoTestScene() override = default;
	BanjoTestScene(const BanjoTestScene& other) = delete;
	BanjoTestScene(BanjoTestScene&& other) noexcept = delete;
	BanjoTestScene& operator=(const BanjoTestScene& other) = delete;
	BanjoTestScene& operator=(BanjoTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;
};

