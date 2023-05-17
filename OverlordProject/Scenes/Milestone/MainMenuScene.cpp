#include "stdafx.h"
#include "MainMenuScene.h"

#include <ranges>
#include "ProjectUtils.h"
#include "Components/ButtonComponent.h"
#include <Misc/json.hpp>

void MainMenuScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = { 0,0,0,0 };


	auto soundManager = SoundManager::Get();

	//SOUND 2D
	auto pFmod = soundManager->GetSystem();

	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/Menus/1-02 Title Theme.mp3", FMOD_DEFAULT || FMOD_LOOP_NORMAL, nullptr, &m_pBackgroundMusic);
	soundManager->ErrorCheck(result);

	
	json menus = *ContentManager::Load<json>(L"GameData/Menus/StartMenus.json");
	
	const size_t menuSize{ menus.size() };
	for(size_t i{}; i < menuSize; ++i)
	{
		json menu{ menus.at(i) };
		std::string menuName = menu["menu"];
		std::string fileExtension = menu["fileExtension"];
		json buttons{ menu.at("buttons") };
		const size_t buttonSize{buttons.size()};
		std::vector<ButtonComponent*> buttonComponents{};

		auto spriteObject{ AddChild(new GameObject()) };

		std::string filePath{ "Textures/Menus/" };

		filePath += menuName;
		filePath += fileExtension;
		std::wstring wsTmp(filePath.begin(), filePath.end());

		auto spriteComp = spriteObject->AddComponent(new SpriteComponent(wsTmp, { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));

		spriteObject->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);
		spriteObject->GetTransform()->Scale(1.f, 1.f, 1.f);

		for(size_t j{}; j < buttonSize; ++j)
		{
			json button = buttons.at(j);
			json pos = button.at("pos");
			const float width{ button["width"] };
			const float height{ button["height"] };
			const XMFLOAT3 pos3{ pos["x"],pos["y"],0.f };

			auto object =  spriteObject->AddChild(new GameObject{});
			auto comp{ object->AddComponent<ButtonComponent>(new ButtonComponent{ width,height }) };

			auto pTransform = object->GetTransform();
			pTransform->Translate(pos3);
			buttonComponents.emplace_back(comp);
		}
		m_MenuButtons.emplace(menuName, buttonComponents);
		m_pSprites.emplace(menuName, spriteComp);
	}

	SetMenu("MainMenu");

	m_MenuButtons["MainMenu"][0]->SetClickEvent([]()
		{
			SceneManager::Get()->SetActiveGameScene(L"BanjoTestScene");
		});

	m_MenuButtons["MainMenu"][1]->SetClickEvent([this]()
		{
			SetMenu("ControlMenu");
		});

	m_MenuButtons["MainMenu"][2]->SetClickEvent([]()
		{
			PostQuitMessage(WM_QUIT);
		});

	m_MenuButtons["ControlMenu"][0]->SetClickEvent([this]()
		{
			SetMenu("MainMenu");
		});
}

void MainMenuScene::OnGUI()
{
	//const auto pSpriteComponent = m_pSprite->GetComponent<SpriteComponent>();

	//ImGui::SliderFloat2("Position", ConvertUtil::ToImFloatPtr(pSpriteComponent->GetTransform()->GetPosition()), 0, m_SceneContext.windowWidth);
	//ImGui::SliderFloat2("Scale", ConvertUtil::ToImFloatPtr(pSpriteComponent->GetTransform()->GetScale()), 0, 3);
	//ImGui::SliderFloat2("Pivot", ConvertUtil::ToImFloatPtr(pSpriteComponent->GetPivot()), 0, 1);
	//ImGui::ColorEdit4("Color", ConvertUtil::ToImFloatPtr(pSpriteComponent->GetColor()));
	//ImGui::Checkbox("AutoRotate", &m_AutoRotate);

	//ImGui::BeginDisabled(m_AutoRotate);
	//ImGui::SliderFloat("Rotation", &m_TotalRotation, 0, 360);
	//ImGui::EndDisabled();
}

void MainMenuScene::Update()
{
}

void MainMenuScene::OnSceneActivated()
{
	const auto soundManager = SoundManager::Get();
	const auto pFmod = soundManager->GetSystem();
	const FMOD_RESULT result = pFmod->playSound(m_pBackgroundMusic, nullptr, false, &m_pChannel2D);
	soundManager->ErrorCheck(result);
}

void MainMenuScene::OnSceneDeactivated()
{
	m_pChannel2D->stop();
}

void MainMenuScene::SetMenu(const std::string& name)
{
	for (const auto& pair : m_MenuButtons)
	{
		bool canDraw = false;
		if(pair.first == name)
		{
			canDraw = true;
		}
		for(auto& button: pair.second)
		{
			button->Enable(canDraw);
		}

		m_pSprites[pair.first]->EnableDrawing(canDraw);
	}
}
