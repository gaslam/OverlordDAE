#include "stdafx.h"
#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(float width, float height) :
m_Width{width},
m_Height{height}
{
}

void ButtonComponent::Initialize(const SceneContext& sceneContext)
{
	auto input = sceneContext.pInput;
	if (!input)
	{
		Logger::LogWarning(L"No input available!! Game will not be controllable!!");
	}

	InputAction mouseLeftClick{ buttonLeftClick,InputState::pressed,-1,VK_LBUTTON };
	sceneContext.pInput->AddInputAction(mouseLeftClick);
}

void ButtonComponent::AddClickSound(std::string& filePath)
{
	auto soundManager = SoundManager::Get();
	auto pFmod = soundManager->GetSystem();
	FMOD_RESULT result = pFmod->createStream(filePath.c_str(), FMOD_DEFAULT, nullptr, &m_pClickSound);
	soundManager->ErrorCheck(result);
	soundManager->ErrorCheck(result);
}

void ButtonComponent::SetClickEvent(const ClickEvent& event)
{
	m_Event = event;
}

void ButtonComponent::Update(const SceneContext& sceneContext)
{
	auto input{ sceneContext.pInput };
	if(!input)
	{
		Logger::LogWarning(L"No input available!! Game will not be controllable!!");
	}

	auto pTransform = GetTransform();
	if(!pTransform)
	{
		Logger::LogWarning(L"No transform available in button!! Game will not be able to register clicks!!");
		return;
	}
	const auto pos = pTransform->GetPosition();
	auto mousePos = input->GetMousePosition();
	const bool isInXRange{ mousePos.x >= pos.x && mousePos.x <= pos.x + m_Width };
	const bool isInYRange{ mousePos.y >= pos.y && mousePos.y <= pos.y + m_Height };

	const bool canClick{ sceneContext.pInput->IsActionTriggered(buttonLeftClick) && isInXRange && isInYRange };

	if(canClick)
	{
		Click();
	}
}

void ButtonComponent::Click()
{
	if(m_pClickSoundChannel && m_pClickSound)
	{
		auto soundManager = SoundManager::Get();
		auto pFmod = soundManager->GetSystem();
		pFmod->playSound(m_pClickSound, nullptr, false, &m_pClickSoundChannel);
	}

	if(m_Event)
	{
		m_Event();
	}
}
