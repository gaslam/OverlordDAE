#include "stdafx.h"
#include "NumberDisplayComponent.h"

NumberDisplayComponent::NumberDisplayComponent(const std::wstring& iconPath) : m_IconPath{iconPath}
{
}

void NumberDisplayComponent::Initialize(const SceneContext& sceneContext)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"Textures/SpriteFonts/MarioAndLuigi_110.fnt");
	m_Text = std::to_string(m_Number);
	m_SceneContext = sceneContext;
	m_TextColor = { 1.f,1.f,1.f,1.f };

	auto gameObject{ GetGameObject() };
	m_pSpriteComponent = gameObject->AddComponent(new SpriteComponent{ m_IconPath, { 0.5f,0.5f }, m_TextColor });
	m_pSpriteComponent->GetTransform()->Scale(1.f, 1.f, 1.f);
}

void NumberDisplayComponent::Update(const SceneContext&)
{
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), m_TextPosition, m_TextColor);
}

void NumberDisplayComponent::DrawImGUI()
{
	if (ImGui::CollapsingHeader("Text"))
	{
		char buffer[256]{};
		m_Text.copy(&buffer[0], 256);
		if (ImGui::InputText("Text", &buffer[0], 256))
		{
			m_Text = std::string(buffer);
		}

		ImGui::SliderFloat2("Position", &m_TextPosition.x, 0, m_SceneContext.windowWidth);
		ImGui::ColorEdit4("Color", &m_TextColor.x, ImGuiColorEditFlags_NoInputs);
	}

	if(ImGui::CollapsingHeader("Sprite"))
	{
		ImGui::SliderFloat2("Position", &m_IconPosition.x, 0, m_SceneContext.windowWidth);
		ImGui::SliderFloat2("Scale", &m_IconScale.x, 0, 3);
		ImGui::SliderFloat2("Pivot", ConvertUtil::ToImFloatPtr(m_pSpriteComponent->GetPivot()), 0, 1);
		ImGui::ColorEdit4("Color", ConvertUtil::ToImFloatPtr(m_pSpriteComponent->GetColor()));
	}
}
