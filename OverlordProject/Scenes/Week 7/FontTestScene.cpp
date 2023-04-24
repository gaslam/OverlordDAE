#include "stdafx.h"
#include "FontTestScene.h"

void FontTestScene::Initialize()
{
	m_pFont = ContentManager::Load<SpriteFont>(L"Textures/SpriteFonts/Consolas_32.fnt");
	LoadScene();
	m_SceneContext.pInput->AddInputAction(InputAction{ InputIds::reset,InputState::pressed,'R' });
}

void FontTestScene::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(InputIds::reset))
	{
		LoadScene();
	}
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), m_TextPosition, m_TextColor);
}

void FontTestScene::OnGUI()
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

void FontTestScene::OnSceneActivated()
{
	LoadScene();
}

void FontTestScene::LoadScene()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.clearColor = XMFLOAT4{ 0.f,0.f,0.f,0.f };
	m_Text = "Hello World! (Change Text/Position/Color in Scene Parameters)";

	m_TextColor = { 1.f,1.f,1.f,1.f };
	m_TextPosition = {};
}
