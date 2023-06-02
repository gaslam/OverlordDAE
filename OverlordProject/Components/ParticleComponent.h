#pragma once
class ParticleComponent :
    public BaseComponent
{
public:
    explicit ParticleComponent(const std::wstring& assetFile) : m_AssetFile{assetFile} {};
    void Initialize(const SceneContext& sceneContext) override;
    void Update(const SceneContext&) override;
    bool GetIsActive() const { return m_IsActive; }
    bool CanBeDeleted() const { return m_CanBeDeleted; }
    void SetIsActive(bool isActive)
    {
	    m_IsActive = isActive;
        m_pEmiter->SetDisabled(false);
    }
    void DrawImGUI() const;
protected:
    void Draw(const SceneContext&) override;
private:
    bool m_CanBeDeleted{false};
    bool m_IsActive{false};
    float m_TimeToBeActive{ 1.f };
    float m_AccumulatedTime{  };
    std::wstring m_AssetFile{};
    ParticleEmitterComponent* m_pEmiter{};
};

