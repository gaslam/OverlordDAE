#pragma once
class CoinObject;
class CoinComponent : public BaseComponent
{
public:
    explicit CoinComponent(GameScene* scene) : m_pScene{ scene } {};
    void Initialize(const SceneContext& sceneContext) override;
    void Update(const SceneContext& /*sceneContext*/) override;
    void RemoveCoins();
private:
    std::vector<CoinObject*> m_pCoins{};
    GameScene* m_pScene{ nullptr };
};

