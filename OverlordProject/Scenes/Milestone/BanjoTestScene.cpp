#include "stdafx.h"
#include "BanjoTestScene.h"
#include "Prefabs/ThirdPersonCamera.h"
#include "ProjectUtils.h"
#include "Prefabs/StarObject.h"
#include "Prefabs/Character.h"
#include "Prefabs/CoinObject.h"

void BanjoTestScene::Initialize()
{
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
	m_pPlayableCharacter->GetTransform()->Translate(-26.92f, 9.36f, -28.67f);

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


	m_SceneContext.settings.enableOnGUI = true;

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

	FMOD::Sound* pSound{};
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/Level1/1_05 Super_Mario_64_Main_Theme.mp3", FMOD_DEFAULT || FMOD_LOOP_NORMAL, nullptr, &pSound);
	soundManager->ErrorCheck(result);
	result = pFmod->playSound(pSound, nullptr, false, &m_pChannel2D);
	soundManager->ErrorCheck(result);
	AddStars();
	AddCoins();
}

void BanjoTestScene::AddStars()
{
	json jsonData = *ContentManager::Load<json>(L"GameData/Stars.json");
	const float scale = jsonData["scale"];
	json positions = jsonData.at("positions");

	size_t positionSize = positions.size();
	for(size_t i{}; i < positionSize; ++i)
	{
		json position = positions.at(i);
		const XMFLOAT3 pos{ position["x"], position["y"], position["z"] };
		auto star = new StarObject{ pos ,scale};
		AddChild(star);
		m_Stars.emplace_back(star);
	}
}

void BanjoTestScene::AddCoins()
{
	json jsonData = *ContentManager::Load<json>(L"GameData/Coins.json");
	const float scale = jsonData["scale"];
	json positions = jsonData.at("positions");

	size_t positionSize = positions.size();
	for (size_t i{}; i < positionSize; ++i)
	{
		json position = positions.at(i);
		const XMFLOAT3 pos{ position["x"], position["y"], position["z"] };
		auto coin = new CoinObject{ pos ,scale };
		AddChild(coin);
		m_Coins.emplace_back(coin);
	}
}


void BanjoTestScene::OnGUI()
{
	if (m_pPlayableCharacter)
	{
		const auto pTransform = m_pPlayableCharacter->GetTransform();

		auto inputFlags = ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat3("Position", ConvertUtil::ToImFloatPtr(pTransform->GetPosition()), "%.2f", inputFlags);
	}
}

void BanjoTestScene::Update()
{
	constexpr bool markForDeletion{ true };
	for (size_t i{}; i < m_Stars.size(); ++i)
	{
		auto star = m_Stars[i];
		if (star->IsMarkedAsDeleted())
		{
			RemoveChild(star, markForDeletion);
			m_Stars.erase(m_Stars.begin() + i);
			--i;
		}
	}

	for (size_t i{}; i < m_Coins.size(); ++i)
	{
		auto coin = m_Coins[i];
		if (coin->IsMarkedAsDeleted())
		{
			RemoveChild(coin, markForDeletion);
			m_Coins.erase(m_Coins.begin() + i);
			--i;
		}
	}
}
