#include "stdafx.h"
#include "GameOverScene.h"

#include "ProjectUtils.h"
#include "Components/ButtonComponent.h"


void GameOverScene::Initialize()
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


	json menu = *ContentManager::Load<json>(L"GameData/Menus/GameOverMenu.json");

	std::string menuName = menu["menu"];
	std::string fileExtension = menu["fileExtension"];
	json buttons{ menu.at("buttons") };
	const size_t buttonSize{ buttons.size() };
	std::vector<ButtonComponent*> buttonComponents{};

	auto spriteObject{ AddChild(new GameObject()) };

	std::string filePath{ "Textures/Menus/" };

	filePath += menuName;
	filePath += fileExtension;
	std::wstring wsTmp(filePath.begin(), filePath.end());

	m_pSprite = spriteObject->AddComponent(new SpriteComponent(wsTmp, { 0.5f,0.5f }, { 1.f,1.f,1.f,.5f }));

	spriteObject->GetTransform()->Translate(m_SceneContext.windowWidth / 2.f, m_SceneContext.windowHeight / 2.f, .9f);
	spriteObject->GetTransform()->Scale(1.f, 1.f, 1.f);

	for (size_t j{}; j < buttonSize; ++j)
	{
		json button = buttons.at(j);
		json pos = button.at("pos");
		const float width{ button["width"] };
		const float height{ button["height"] };
		const XMFLOAT3 pos3{ pos["x"],pos["y"],0.f };

		auto object = spriteObject->AddChild(new GameObject{});
		auto comp{ object->AddComponent<ButtonComponent>(new ButtonComponent{ width,height }) };

		auto pTransform = object->GetTransform();
		pTransform->Translate(pos3);
		buttonComponents.emplace_back(comp);
	}
	m_MenuButtons.emplace(menuName, buttonComponents);

	m_MenuButtons["GameOverMenu"][0]->SetClickEvent([]()
		{
			SceneManager::Get()->SetActiveGameScene(L"MainMenuScene");
		});

	m_MenuButtons["GameOverMenu"][1]->SetClickEvent([this]()
		{
			PostQuitMessage(WM_QUIT);
		});
}

void GameOverScene::OnSceneActivated()
{
	const auto soundManager = SoundManager::Get();
	const auto pFmod = soundManager->GetSystem();
	const FMOD_RESULT result = pFmod->playSound(m_pBackgroundMusic, nullptr, false, &m_pChannel2D);
	soundManager->ErrorCheck(result);
}

void GameOverScene::OnSceneDeactivated()
{
	m_pChannel2D->stop();
}