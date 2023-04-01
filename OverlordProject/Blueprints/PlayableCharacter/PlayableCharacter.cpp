#include "stdafx.h"
#include "PlayableCharacter.h"
#include "Components/CharacterComponent.h"
#include <Materials/DiffuseMaterial_Skinned.h>
#include "Prefabs/ThirdPersonCamera.h"


PlayableCharacter::PlayableCharacter()
{
	const UINT8 totalMaterials{ 6 };
	std::wstring characterTextures[totalMaterials]{ L"banjo_feet.png",L"banjo_belt.png",L"banjo_nose.png",L"banjo_eyes.png",L"banjo_backpack.png",L"banjo_color.png" };
	std::wstring modelPath{ L"Meshes/Characters/Banjo/banjo.ovm" };
	std::wstring texturePath{ L"Textures/" };
	std::vector<DiffuseMaterial_Skinned*> materials{};
	materials.resize(totalMaterials);
	for (auto& material : materials)
	{
		material = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	}
	m_pModelComponent = ModelUtil::CreateModelWithTexturesAndMaterials(materials, modelPath, texturePath, characterTextures, totalMaterials);
}

void PlayableCharacter::SetAnimator(ModelAnimator* pAnimator)
{
	auto pCharacterComp = GetComponent<CharacterComponent>();
	if (pCharacterComp)
	{
		pCharacterComp->SetAnimator(pAnimator);
	}
}

void PlayableCharacter::Initialize(const SceneContext& sceneContext)
{
	AddComponent(m_pModelComponent);
	AddComponent(new CharacterComponent{ m_pModelComponent->GetAnimator(),m_Scale });
	InputAction actionMoveUp{ Actions::moveForward,InputState::down,'W', -1,XINPUT_GAMEPAD_DPAD_UP };
	InputAction actionMoveDown{ Actions::moveBackwards,InputState::down,'S',-1,XINPUT_GAMEPAD_DPAD_DOWN };
	InputAction actionMoveLeft{ Actions::moveLeft,InputState::down,'A',-1,XINPUT_GAMEPAD_DPAD_LEFT };
	InputAction actionMoveRight{ Actions::moveRight,InputState::down,'D',-1,XINPUT_GAMEPAD_DPAD_RIGHT };
	sceneContext.pInput->AddInputAction(actionMoveUp);
	sceneContext.pInput->AddInputAction(actionMoveDown);
	sceneContext.pInput->AddInputAction(actionMoveLeft);
	sceneContext.pInput->AddInputAction(actionMoveRight);
	m_pCamera = new ThirdPersonCamera(GetTransform(), 20.f);
	m_TotalYaw = m_pCamera->GetYaw();
}

void PlayableCharacter::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	HandleMovement(sceneContext, deltaTime);
}

void PlayableCharacter::HandleButtons(float deltaTime, float& targetAngle, InputManager* pInput, TransformComponent* pCharacterTransform, bool& pressed)
{
	bool isCharacterMoving{ false };
	bool isMovingBackwards{ false };
	bool isMovingForwards{ false };
	XMFLOAT2 dir{};

	auto pCamTransform = m_pCamera->GetTransform();
	if (!pCamTransform)
	{
		return;
	}

	if (pInput->IsActionTriggered(Actions::moveForward))
	{
		dir.x = 1.f;
		isCharacterMoving = true;
		isMovingForwards = true;
	}

	if (pInput->IsActionTriggered(Actions::moveBackwards))
	{
		dir.x = -1.f;
		targetAngle -= 180.f;
		isCharacterMoving = true;
		isMovingBackwards = true;
	}

	float AngleToAdd = isMovingForwards || isMovingBackwards ? 45.f : 90.f;

	if (pInput->IsActionTriggered(Actions::moveLeft))
	{
		dir.y = -1.f;
		targetAngle -= AngleToAdd;
		isCharacterMoving = true;
	}

	if (pInput->IsActionTriggered(Actions::moveRight))
	{
		dir.y = 1.f;
		targetAngle += AngleToAdd;
		isCharacterMoving = true;
	}
	pressed = isCharacterMoving;

	if (!isCharacterMoving)
	{
		return;
	}

	auto pos = XMLoadFloat3(&pCharacterTransform->GetPosition());
	auto forward = pCamTransform->GetForward();
	forward.y = 0.f;
	auto forwardVec = XMLoadFloat3(&forward);
	auto right = pCamTransform->GetRight();
	right.y = 0.f;
	auto rightVec = XMLoadFloat3(&right);

	pos += forwardVec * dir.x * m_CharacterSpeed * deltaTime;
	pos += rightVec * dir.y * m_CharacterSpeed * deltaTime;

	pCharacterTransform->GetTransform()->Translate(pos);
}

void PlayableCharacter::HandleMovement(const SceneContext& sceneContext, float deltaTime)
{
	InputManager* pInput = sceneContext.pInput;
	auto pCameraComponent = m_pCamera->GetComponent<CameraComponent>();

	if (!pCameraComponent || !pCameraComponent->IsActive())
	{
		return;
	}

	m_TargetFowardRotationYaw = m_pCamera->GetYaw();
	bool isCharacterMoving = false;
	auto pTransform = GetComponent<CharacterComponent>()->GetTransform();
	HandleButtons(deltaTime, m_TargetFowardRotationYaw, pInput,pTransform, isCharacterMoving);
	if (m_TargetFowardRotationYaw != m_TotalYaw && isCharacterMoving)
	{
		float angleToAdd{ m_TargetFowardRotationYaw > m_TotalYaw ? deltaTime * m_AngularSpeed : -deltaTime * m_AngularSpeed };
		m_TotalYaw += angleToAdd;
	}
	pTransform->Rotate(0.f, m_TotalYaw, 0.f);
}
