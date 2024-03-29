#pragma once
#include <vector>
class PostPixelate;
class Character;
class StarObject;
class CoinObject;
class StarComponent;
class CoinComponent;
class BomOmbBattlefield :
    public GameScene
{
public:
	BomOmbBattlefield() : GameScene(L"Bomb-omb Battlefield") {}
	~BomOmbBattlefield() override = default;
	BomOmbBattlefield(const BomOmbBattlefield& other) = delete;
	BomOmbBattlefield(BomOmbBattlefield&& other) noexcept = delete;
	BomOmbBattlefield& operator=(const BomOmbBattlefield& other) = delete;
	BomOmbBattlefield& operator=(BomOmbBattlefield&& other) noexcept = delete;

	void End() override;
protected:
	void Initialize() override;
	void OnGUI() override;
	void Update() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
private:
	Character* m_pPlayableCharacter{ nullptr };
	StarComponent* m_pStarComponent{ nullptr };
	CoinComponent* m_pCoinComponent{ nullptr };
	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pBackgroundMusic{ nullptr };

	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		reset
	};

	void AddCollectibles();
	PostPixelate* m_Pixelate{ nullptr };

	void LoadScene();
};

