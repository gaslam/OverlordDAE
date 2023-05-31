#pragma once
#include "Observers/AmountObserver.h"
class CoinObject;
class CoinComponent : public BaseComponent
{
public:
    explicit CoinComponent() {};
    void Initialize(const SceneContext& sceneContext) override;
    void Update(const SceneContext& /*sceneContext*/) override;
    void RemoveCoins();
    int GetCoinsCount() const { return m_CoinsCollected; }
    void OnGUI();
    void InitObjects();
private:
    int m_CoinsCollected{};
    GameObject* m_NumberDisplayObject{};
    std::vector<CoinObject*> m_pCoins{};
    std::unique_ptr<AmountObserverCoins> m_AmountObserver{};
    void AddObjects();
};

