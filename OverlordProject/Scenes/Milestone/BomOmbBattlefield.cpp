#include "stdafx.h"
#include "BomOmbBattlefield.h"
#include "Prefabs/ThirdPersonCamera.h"
#include "ProjectUtils.h"
#include "Components/CoinComponent.h"
#include "Components/BallComponent.h"
#include "Components/SpawnerComponent.h"
#include "Components/StarComponent.h"
#include "Materials/Post/PostPixelate.h"
#include "Prefabs/Character.h"

FMOD_RESULT F_CALLBACK channelEndCallback(FMOD_CHANNELCONTROL*, FMOD_CHANNELCONTROL_TYPE, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,void*,void*)
{
	if (callbackType == FMOD_CHANNELCONTROL_CALLBACK_END)
	{

		// Trigger your event or perform other actions
		auto sceneManager = SceneManager::Get();
		sceneManager->SetActiveGameScene(L"GameOverScene");
	}

	return FMOD_OK;
}

void BomOmbBattlefield::End()
{
 	bool isPlaying{};
	m_pChannel2D->isPlaying(&isPlaying);
	if(isPlaying)
	{
		m_pChannel2D->stop();
	}
	const auto soundManager = SoundManager::Get();
	const auto pFmod = soundManager->GetSystem();
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/Level1/1-25 Course Clear.mp3", FMOD_DEFAULT || FMOD_LOOP_NORMAL, nullptr, &m_pBackgroundMusic);
	soundManager->ErrorCheck(result);
	pFmod->playSound(m_pBackgroundMusic, nullptr, false, &m_pChannel2D);

	m_pChannel2D->setCallback(channelEndCallback);
}

void BomOmbBattlefield::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawPhysXDebug = false;

	m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;
	characterDesc.JumpSpeed = 10.f;
	characterDesc.controller.height = 0.4f;
	characterDesc.controller.radius = 0.3f;
	characterDesc.moveAccelerationTime = 2.f;
	characterDesc.maxMoveSpeed = 13.f;
	characterDesc.controller.density = 20.f;
	characterDesc.rotationSpeed = 20.f;
	auto character = new Character{ characterDesc };
	m_pPlayableCharacter = AddChild(character);

	auto mario = new GameObject();
	ModelComponent* modelCompCharacter = mario->AddComponent(new ModelComponent{ L"Meshes/Characters/Mario/Mario.ovm" });
	std::wstring playerTexturePath = std::wstring{ L"Textures/Characters/Mario/" };
	std::wstring textureExtension = L"png";
	character->AddChild(mario);
	mario->GetTransform()->Scale(5.f);
	mario->GetTransform()->Translate(0.f, -characterDesc.controller.height - 0.1f, 0.f);
	character->SetAnimator(modelCompCharacter->GetAnimator());
	character->GetTransform()->Scale(1.f, 1.f, 1.f);
	m_pPlayableCharacter->GetTransform()->Translate(-0.12f, 21.79f, 27.71f);

	ModelUtils::AddTextureToModelComponent(modelCompCharacter, L"Textures/Characters/Mario/textureData.json");


	ModelComponent* modelComponentEnv = new ModelComponent{ L"Meshes/Environments/Level_1_Body.ovm" };
	auto object = new GameObject{};
	object->AddComponent<ModelComponent>(modelComponentEnv);
	m_pPlayableCharacter->GetChild<ThirdPersonCamera>()->AddModelToAvoid(modelComponentEnv);
	AddChild(object);


	ModelUtils::AddTextureToModelComponent(modelComponentEnv,L"Textures/Environments/Level_1/Body/textureData.json");

	ModelComponent* modelComponentFences = new ModelComponent{ L"Meshes/Environments/Level_1_Fences.ovm" };
	auto object2 = new GameObject{};
	object2->AddComponent<ModelComponent>(modelComponentFences);
	object->AddChild(object2);


	ModelUtils::AddTextureToModelComponent(modelComponentFences, L"Textures/Environments/Level_1/Fences/textureData.json");

	auto& physx = PxGetPhysics();
	auto physxMat = physx.createMaterial(10.f, 10.f, 0.5f);
	auto pTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Environments/Level_1.ovpt");
	auto pRigidBody = object->AddComponent<RigidBodyComponent>(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxTriangleMeshGeometry{ pTriangleMesh }, *physxMat);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);

	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	auto soundManager = SoundManager::Get();
	//SOUND 2D
	auto pFmod = soundManager->GetSystem();

	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/Level1/1_05 Super_Mario_64_Main_Theme.mp3", FMOD_DEFAULT || FMOD_LOOP_NORMAL, nullptr, &m_pBackgroundMusic);
	soundManager->ErrorCheck(result);
	AddCollectibles();
	XMFLOAT3 spawnerPos{ 0,0,0 };
	GameObject* ballsObject{ AddChild(new GameObject() )};
	XMFLOAT3 pos{ 0,0,0 };
	ballsObject->AddComponent(new BallComponent{});

	m_Pixelate = MaterialManager::Get()->CreateMaterial<PostPixelate>();
	const XMFLOAT2 pixels{ 1024.f,576.f};
	m_Pixelate->SetPixels(pixels);
	//AddPostProcessingEffect(m_Pixelate);

	m_Pixelate->SetIsEnabled(true);
}

void BomOmbBattlefield::AddCollectibles()
{
	GameObject* starBaseObject = new GameObject();
	GameObject* coinBaseObject = new GameObject();
	m_pStarComponent = starBaseObject->AddComponent(new StarComponent{});
	coinBaseObject->AddComponent(new CoinComponent{this});
	AddChild(starBaseObject);
	AddChild(coinBaseObject);
}


void BomOmbBattlefield::OnGUI()
{
	if (m_pPlayableCharacter)
	{
		m_pPlayableCharacter->DrawImGui();
	}

	if(m_Pixelate)
	{
		bool isEnabled = m_Pixelate->IsEnabled();
		ImGui::Checkbox("Pixelate PP", &isEnabled);
		m_Pixelate->SetIsEnabled(isEnabled);
	}

	if(m_pStarComponent)
	{
		m_pStarComponent->OnGUI();
	}
}

void BomOmbBattlefield::Update()
{
}

void BomOmbBattlefield::OnSceneActivated()
{
	const auto soundManager = SoundManager::Get();
	const auto pFmod = soundManager->GetSystem();
	const FMOD_RESULT result = pFmod->playSound(m_pBackgroundMusic, nullptr, false, &m_pChannel2D);
	soundManager->ErrorCheck(result);
}

void BomOmbBattlefield::OnSceneDeactivated()
{
	m_pChannel2D->stop();
}
