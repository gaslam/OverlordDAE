#include "stdafx.h"
#include "CharacterScene.h"

#include "Prefabs/Character.h"
#include "Materials/ColorMaterial.h"

void CharacterScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;

	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//Character
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;

	m_pCharacter = AddChild(new Character(characterDesc));
	m_pCharacter->GetTransform()->Translate(0.f, 5.f, 0.f);

	//Simple Level
	const auto pLevelObject = AddChild(new GameObject());
	const auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Environments/SimpleLevel.ovm"));
	pLevelMesh->SetMaterial(MaterialManager::Get()->CreateMaterial<ColorMaterial>());

	const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Environments/SimpleLevel.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ .5f,.5f,.5f })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale(.5f, .5f, .5f);

	//Input
	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(reset, InputState::pressed, 'R');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);
}

void CharacterScene::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(reset))
	{
		LoadScene();
	}
}

void CharacterScene::OnGUI()
{
	m_pCharacter->DrawImGui();
}

void CharacterScene::OnSceneActivated()
{
	LoadScene();
}

void CharacterScene::LoadScene()
{
	CharacterDesc desc = m_pCharacter->GetCharacterDesc();
	desc.maxMoveSpeed = 15.f;
	desc.moveAccelerationTime = 0.3f;
	desc.maxFallSpeed = 15.f;
	desc.fallAccelerationTime = 0.3f;
	desc.JumpSpeed = 15.f;
	desc.rotationSpeed = 60.f;
	m_pCharacter->SetCharacterDesc(desc);
	m_pCharacter->GetTransform()->Translate(0.f, 5.f, 0.f);
	m_pCharacter->SetYaw(0.f);
	m_pCharacter->SetPitch(0.f);
	
}
