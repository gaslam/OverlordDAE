#pragma once
#include <vector>
#include <algorithm>
class ModelComponent;
class PlayableCharacter;
class EnvironmentComponent :
    public BaseComponent
{
public:
    explicit EnvironmentComponent(PlayableCharacter* pCharacter);
    void AddEnvironment(ModelComponent* pEnvironment)
    {
        if (pEnvironment == nullptr)
        {
            Logger::LogWarning(L"Cannot add an environment as nullptr!!");
            return;
        }
        
        if (!pEnvironment->HasMeshFilter())
        {
            Logger::LogWarning(L"Cannot add an enviroment without meshes!!");
            return;
        }
        m_Environments.push_back(pEnvironment);
    }

    void RemoveEnvironment(ModelComponent* pComponent)
    {
        auto it = std::remove(m_Environments.begin(), m_Environments.end(), pComponent);
        if (it != m_Environments.end())
        {
            m_Environments.erase(it);
        }
    }

    void Update(const SceneContext& context) override;
private:
    std::vector<ModelComponent*> m_Environments{};
    PlayableCharacter* m_pPlayableCharacter{};
};

