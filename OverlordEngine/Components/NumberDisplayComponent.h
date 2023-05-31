#pragma once
#include "BaseComponent.h"
class NumberDisplayComponent :
    public BaseComponent
{

public:
    explicit NumberDisplayComponent(const std::wstring& iconPath);
    void Initialize(const SceneContext& sceneContext) override;
    void Update(const SceneContext&) override;
    void SetNumber(int number)
    {
	    m_Number = number;
        m_Text = std::to_string(m_Number);
    }

    void DrawImGUI();

    void SetTextPosition(XMFLOAT2& pos) { m_TextPosition = pos; }
    void SetIconPosition(XMFLOAT2& pos)
    {
	    m_IconPosition = pos;
        m_pSpriteComponent->GetTransform()->Translate(m_IconPosition.x, m_IconPosition.y, 0.f);
    }

    void SetIconScale(XMFLOAT2& scale)
    {
        m_IconScale = scale;
        m_pSpriteComponent->GetTransform()->Scale(scale.x, scale.y, 0.f);
    }

private:
    SpriteFont* m_pFont{};
    SpriteComponent* m_pSpriteComponent{};
    int m_Number{ 0 };

    std::string m_Text{  };
    XMFLOAT2 m_TextPosition{};
    XMFLOAT2 m_IconPosition{};
    XMFLOAT2 m_IconScale{};
    XMFLOAT4 m_TextColor{ };

    SceneContext m_SceneContext{};
    std::wstring m_IconPath{};
};

