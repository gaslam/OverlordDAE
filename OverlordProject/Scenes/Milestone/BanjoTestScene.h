#pragma once
#include <vector>
class Character;
class StarObject;
class CoinObject;
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
	void Update() override;
private:
	Character* m_pPlayableCharacter{ nullptr };
	FMOD::Channel* m_pChannel2D{ nullptr };

	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		reset
	};

	void AddStars();
	void AddCoins();

	std::vector<StarObject*> m_Stars{};
	std::vector<CoinObject*> m_Coins{};
};

