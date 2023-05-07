#include "stdafx.h"
#include "BanjoTestScene.h"
#include <Materials/DiffuseMaterial.h>
#include "Prefabs/ThirdPersonCamera.h"
#include "ProjectUtils.h"
#include <Materials/DiffuseMaterial_Skinned.h>
#include "Prefabs/Character.h"

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
	mario->GetTransform()->Translate(0.f,-characterDesc.controller.height - 0.1f,0.f);
	character->SetAnimator(modelCompCharacter->GetAnimator());
	character->GetTransform()->Scale(1.f, 1.f, 1.f);
	m_pPlayableCharacter->GetTransform()->Translate(-26.92f, 9.36f, -28.67f);
	std::vector<std::wstring> textureNames{ L"Mario64Eye_alb.0",L"Mario64Hand_alb",L"Mario64Body_alb",L"Mario64Face_alb",L"Mario64Cap_alb",L"Mario64Cap_alb",L"Mario64Cap_alb",L"Mario64Hand_alb" };
	ModelUtils::AddTexturesToModelComponent<DiffuseMaterial_Skinned>(mario->GetComponent<ModelComponent>(), playerTexturePath,textureNames, textureExtension);
	

	ModelComponent* modelComponentEnv = new ModelComponent{L"Meshes/Environments/Level_1.ovm"};
	auto object = new GameObject{};
	object->AddComponent<ModelComponent>(modelComponentEnv);
	m_pPlayableCharacter->GetChild<ThirdPersonCamera>()->AddModelToAvoid(modelComponentEnv);
	AddChild(object);

	std::wstring textureFolder = L"Textures/Environments/Level_1/";

	ModelUtils::AddTexturesToModelComponent<DiffuseMaterial>(modelComponentEnv,textureFolder, textureExtension);
	textureExtension = L"bmp";
	ModelUtils::AddTexturesToModelComponent<DiffuseMaterial>(modelComponentEnv,textureFolder, textureExtension);
	textureExtension = L"jpg";
	ModelUtils::AddTexturesToModelComponent<DiffuseMaterial>(modelComponentEnv, textureFolder, textureExtension);

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
}

void BanjoTestScene::OnGUI()
{
	if (m_pPlayableCharacter)
	{
		const auto pTransform = m_pPlayableCharacter->GetTransform();

		auto inputFlags = ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat3("Position", ConvertUtil::ToImFloatPtr(pTransform->GetPosition()),"%.2f", inputFlags);
	}
}
