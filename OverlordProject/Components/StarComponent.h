#pragma once
#include <vector>
#include "Observers/AmountObserver.h"
class StarObject;

class StarComponent : public BaseComponent
{
public:
    explicit StarComponent()  {};
    void Initialize(const SceneContext& sceneContext) override;
    void Update(const SceneContext& /*sceneContext*/) override;
    void RemoveStars();
    int GetStarCount() const { return m_StarsCollected; }
    void OnGUI();
private:
    int m_StarsCollected{};
    GameObject* m_NumberDisplayObject{};
    std::vector<StarObject*> m_pStars{};
    std::unique_ptr<AmountObserver> m_AmountObserver{};
};	
