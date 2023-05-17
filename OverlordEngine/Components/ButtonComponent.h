#pragma once
#include "BaseComponent.h"
#include <OverlordAPI.h>
class ButtonComponent :
    public BaseComponent
{
    using ClickEvent = std::function<void()>;
public:
    ButtonComponent(float width, float height);
    void Initialize(const SceneContext& sceneContext) override;
    void AddClickSound(std::string& filePath);
    void SetClickEvent(const ClickEvent& event);
    void Update(const SceneContext& sceneContext) override;
    void Enable(const bool isEnabled) { m_IsEnabled = isEnabled; };
private:
    const float m_Width{};
    const float m_Height{};
	bool m_IsEnabled{};
    FMOD::Channel* m_pClickSoundChannel{};
    FMOD::Sound* m_pClickSound{};
    ClickEvent m_Event;

    enum InputIds
    {
        buttonLeftClick,
    };

    void Click();
};

