#pragma once
#include <vector>

class StarObject;

class StarComponent : public BaseComponent
{
public:
    explicit StarComponent(GameScene* scene) : m_pScene{ scene } {};
    void Initialize(const SceneContext& sceneContext) override;
    void Update(const SceneContext& /*sceneContext*/) override;
    void RemoveStars();
private:
    std::vector<StarObject*> m_pStars;
    GameScene* m_pScene{ nullptr };
};	


