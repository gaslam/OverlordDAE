#pragma once
#include <vector>

class StarObject;

class StarComponent : public BaseComponent
{
public:
    explicit StarComponent()  {};
    void Initialize(const SceneContext& sceneContext) override;
    void Update(const SceneContext& /*sceneContext*/) override;
    void RemoveStars();
private:
    std::vector<StarObject*> m_pStars;
};	


