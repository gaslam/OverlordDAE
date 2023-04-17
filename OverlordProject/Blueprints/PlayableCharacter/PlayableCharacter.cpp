#include "stdafx.h"
#include "PlayableCharacter.h"
#include "Components/CharacterComponent.h"
#include <Materials/DiffuseMaterial_Skinned.h>
#include "Prefabs/ThirdPersonCamera.h"
#include "ProjectUtils.h"


PlayableCharacter::PlayableCharacter()
{
	std::wstring fileExtension{ L"png" };

	m_pModelComponent = new ModelComponent{ L"Meshes/Characters/Banjo/banjo.ovm" };
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
	m_TotalYaw = m_pCamera->GetYaw() - 180.f;
}

void PlayableCharacter::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.pGameTime->GetElapsed();
	HandleMovement(sceneContext, deltaTime);
}

void PlayableCharacter::HandleButtons(float deltaTime, float& targetAngle, InputManager* pInput, TransformComponent*, bool& pressed)
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
		targetAngle -= isMovingBackwards ? -AngleToAdd : AngleToAdd;
		isCharacterMoving = true;
	}

	if (pInput->IsActionTriggered(Actions::moveRight))
	{
		dir.y = 1.f;
		targetAngle += isMovingBackwards ? -AngleToAdd : AngleToAdd;
		isCharacterMoving = true;
	}
	pressed = isCharacterMoving;

	if (!isCharacterMoving)
	{
		return;
	}

	auto pos = XMVECTOR{};
	auto forward = pCamTransform->GetForward();
	auto forwardVec = XMLoadFloat3(&forward);
	auto right = pCamTransform->GetRight();
	auto rightVec = XMLoadFloat3(&right);

	pos += forwardVec * dir.x  *10000 * deltaTime;
	pos += rightVec * dir.y * 10000 * deltaTime;

	auto component = GetComponent<RigidBodyComponent>();
	XMFLOAT3 posFl;
	XMStoreFloat3(&posFl, pos);
	component->AddForce(posFl);

	if (m_TargetFowardRotationYaw != m_TotalYaw && isCharacterMoving)
	{
		auto runState = CharacterState::Running;
		float angleToAdd{ m_TargetFowardRotationYaw > m_TotalYaw ? deltaTime * m_AngularSpeed : -deltaTime * m_AngularSpeed };
		m_TotalYaw += angleToAdd;
		ChangeState(runState);
	}
}

void PlayableCharacter::HandleMovement(const SceneContext& sceneContext, float deltaTime)
{
	InputManager* pInput = sceneContext.pInput;
	auto pCameraComponent = m_pCamera->GetComponent<CameraComponent>();

	if (!pCameraComponent || !pCameraComponent->IsActive())
	{
		return;
	}

	m_TargetFowardRotationYaw = m_pCamera->GetYaw() - 180.f;
	bool isCharacterMoving = false;
	auto pTransform = GetComponent<CharacterComponent>()->GetTransform();
	HandleButtons(deltaTime, m_TargetFowardRotationYaw, pInput,pTransform, isCharacterMoving);
	if (!isCharacterMoving)
	{
		HandleRotation(deltaTime, m_TargetFowardRotationYaw, pTransform, isCharacterMoving);
	}

	if (!isCharacterMoving)
	{
		auto idleState = CharacterState::Idle;
		ChangeState(idleState);
	}
	pTransform->Rotate(0.f, m_TotalYaw, 0.f);
}

void PlayableCharacter::HandleRotation(float deltaTime, float& targetAngle, TransformComponent*, bool& isMoving)
{
	auto runState = CharacterState::Running;
	auto dir = InputManager::GetThumbstickPosition();
	const bool isThumbStickUsed{ dir.x != 0.f || dir.y != 0.f };
	if (!isThumbStickUsed)
	{
		isMoving = false;
		return;
	}

	auto pCamTransform = m_pCamera->GetTransform();
	if (!pCamTransform)
	{
		isMoving = false;
		return;
	}

	XMVECTOR thumbStickDirVec = XMLoadFloat2(&dir);
	XMVECTOR zeroVector{};

	float angleToSubtract =90.f - ((atan2f(dir.y, dir.x) * 180.f)/ 3.14159265358979323846f);
	targetAngle += angleToSubtract;
	isMoving = true;

	auto pos = XMVECTOR{};
	auto forward = pCamTransform->GetForward();
	forward.y = 0.f;
	auto forwardVec = XMLoadFloat3(&forward);
	auto right = pCamTransform->GetRight();
	right.y = 0.f;
	auto rightVec = XMLoadFloat3(&right);

	pos += forwardVec * dir.y * m_CharacterSpeed * deltaTime;
	pos += rightVec * dir.x * m_CharacterSpeed * deltaTime;

	auto component = GetComponent<RigidBodyComponent>();
	XMFLOAT3 posFl;
	XMStoreFloat3(&posFl, pos);
	component->AddForce(posFl);

	ChangeState(runState);
	if (m_TargetFowardRotationYaw != m_TotalYaw && isMoving)
	{
		if (m_TotalYaw > m_TargetFowardRotationYaw)
		{
			m_TotalYaw -= m_AngularSpeed * deltaTime;
		}

		if (m_TotalYaw <= m_TargetFowardRotationYaw)
		{
			m_TotalYaw += m_AngularSpeed * deltaTime;
		}
		ChangeState(runState);
	}
}

void PlayableCharacter::ChangeState(CharacterState& state)
{
	auto pCharacterComp = GetComponent<CharacterComponent>();
	if (pCharacterComp && m_State != state)
	{
		m_State = state;
		auto pAnimator = pCharacterComp->GetAnimator();
		if (pAnimator)
		{
			pAnimator->SetAnimation(m_State);
		}
	}
}
