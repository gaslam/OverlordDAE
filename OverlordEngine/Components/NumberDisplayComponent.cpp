#include "stdafx.h"
#include "NumberDisplayComponent.h"

NumberDisplayComponent::NumberDisplayComponent(const std::wstring& iconPath) : m_IconPath{iconPath}
{
}

void NumberDisplayComponent::Initialize(const SceneContext&sceneContext)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"Textures/SpriteFonts/MarioAndLuigi_110.fnt");
	m_Text = std::to_string(m_Number);
	m_SceneContext = sceneContext;
	m_TextColor = { 1.f,1.f,1.f,1.f };

	auto gameObject = GetGameObject();
	m_pSpriteComponent = gameObject->AddComponent(new SpriteComponent(m_IconPath, { 0.5f,0.5f }, { 1.f,1.f,1.f,1.f }));

	m_pSpriteComponent->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);
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
		auto pos = m_pSpriteComponent->GetTransform()->GetPosition();
		ImGui::SliderFloat2("Position", ConvertUtil::ToImFloatPtr(pos), 0, m_SceneContext.windowWidth);
		ImGui::SliderFloat2("Scale", ConvertUtil::ToImFloatPtr(m_pSpriteComponent->GetTransform()->GetScale()), 0, 3);
		ImGui::SliderFloat2("Pivot", ConvertUtil::ToImFloatPtr(m_pSpriteComponent->GetPivot()), 0, 1);
		ImGui::ColorEdit4("Color", ConvertUtil::ToImFloatPtr(m_pSpriteComponent->GetColor()));
		m_pSpriteComponent->GetTransform()->Translate(pos);
	}
}
