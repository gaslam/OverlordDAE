#include "stdafx.h"
#include "Character.h"
#include "Prefabs/ThirdPersonCamera.h"

Character::Character(const CharacterDesc& characterDesc) :
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
{
}

void Character::Initialize(const SceneContext& /*sceneContext*/)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	//Camera
	const auto pCamera = AddChild(new ThirdPersonCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera
	auto soundManager = SoundManager::Get();
	auto pSoundSystem = soundManager->GetSystem();
	FMOD_RESULT result = pSoundSystem->createSound("Resources/Sounds/Mario/sm64_mario_hoo.wav", FMOD_DEFAULT, nullptr, &m_pHooSound);
	soundManager->ErrorCheck(result);
	result = pSoundSystem->createSound("Resources/Sounds/Mario/sm64_mario_hoohoo.wav", FMOD_DEFAULT, nullptr, &m_pWooHooSound);
	soundManager->ErrorCheck(result);
	m_pChannel3D->set3DMinMaxDistance(0.f, 100.f);
}

inline FMOD_VECTOR ToFmod(XMFLOAT3 v)
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

void Character::Update(const SceneContext& sceneContext)
{
	if (!m_pCameraComponent->IsActive())
	{
		return;
	}
	constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero
	const float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	const bool moveForward = sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward);
	bool moveBackward = false;
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
	{
		moveBackward = true;
	}
	const bool moveRight = sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight);
	const bool moveLeft = sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft);


	//***************
	//HANDLE INPUT

	//## Input Gathering (move)
	XMFLOAT2 move{}; //Uncomment
	//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
	//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement

	move.y = moveForward ? 1.f : move.y;
	move.y = moveBackward ? -1.f : move.y;
	move.x = moveRight ? 1.f : move.x;
	move.x = moveLeft ? -1.f : move.x;

	if (abs(move.y) < epsilon && abs(move.x) < epsilon)
	{
		move = sceneContext.pInput->GetThumbstickPosition();
	}

	//## Input Gathering (look)
	//Only if the Left Mouse Button is Down >
		// Store the MouseMovement in the local 'look' variable (cast is required)
	//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look

	//************************
	//GATHERING TRANSFORM INFO

	//Retrieve the TransformComponent
	//Retrieve the forward & right vector (as XMVECTOR) from the TransformComponent

	auto pTransform = m_pCameraComponent->GetTransform();
	auto forwardVec = XMLoadFloat3(&pTransform->GetForward());
	auto pRight = XMLoadFloat3(&pTransform->GetRight());

	XMVECTOR moveDir{};
	moveDir += forwardVec * move.y;
	moveDir += pRight * move.x;

	//***************
	//CAMERA ROTATION

	//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable
	//Make sure this calculated on a framerate independent way and uses CharacterDesc::rotationSpeed.
	//Rotate this character based on the TotalPitch (X) and TotalYaw (Y)

	m_TotalYaw = move.x * m_CharacterDesc.rotationSpeed;
	m_TotalPitch = move.y * m_CharacterDesc.rotationSpeed;
	//********
	//MOVEMENT

	//## Horizontal Velocity (Forward/Backward/Right/Left)
	//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
	const float acceleration = m_MoveAcceleration * elapsedTime;
	const bool isMoving = move.x != 0 || move.y != 0;
	if (isMoving)
	{
		m_CurrentDirection = XMFLOAT3{ XMVectorGetX(moveDir),0,XMVectorGetZ(moveDir) };
		m_MoveSpeed += acceleration;

		auto children = GetChildren<GameObject>();
		auto characterForward = GetTransform()->GetForward();
		for (auto child : children)
		{
			const float pi = 3.1415926535f;
			float angle = atan2(-characterForward.z, -characterForward.x) - atan2(m_TotalVelocity.z, m_TotalVelocity.x);
			angle *= (180 / pi);
			child->GetTransform()->Rotate(0, angle, 0);
		}
		if (m_State != Running)
		{
			m_State = Running;
			m_pModelAnimator->SetAnimation(m_State);
		}
	}
	else
	{
		m_MoveSpeed -= acceleration;
	}

	if (m_MoveSpeed > m_CharacterDesc.maxMoveSpeed)
	{
		m_MoveSpeed = m_CharacterDesc.maxMoveSpeed;
	}

	if (m_MoveSpeed < 0.f)
	{
		m_MoveSpeed = 0;
	}

	//If the character is moving (= input is pressed)
		//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input
		//Increase the current MoveSpeed with the current Acceleration (m_MoveSpeed)
		//Make sure the current MoveSpeed stays below the maximum MoveSpeed (CharacterDesc::maxMoveSpeed)
	//Else (character is not moving, or stopped moving)
		//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
		//Make sure the current MoveSpeed doesn't get smaller than zero

	const XMVECTOR velocity = XMLoadFloat3(&m_CurrentDirection) * m_MoveSpeed;

	m_TotalVelocity.x = XMVectorGetX(velocity);
	m_TotalVelocity.z = XMVectorGetZ(velocity);

	//Now we can calculate the Horizontal Velocity which should be stored in m_TotalVelocity.xz
	//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)
	//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
	//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)

	if (!m_pControllerComponent->GetCollisionFlags().isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		m_TotalVelocity.y -= elapsedTime * m_FallAcceleration;
		if (m_TotalVelocity.y < -m_CharacterDesc.maxFallSpeed)
		{
			m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;
		}
	}
	else
	{
		m_TotalVelocity.y = 0;
		m_TimesJumped = 0;
		if (m_State != Idle && !isMoving)
		{
			m_State = Idle;
			m_pModelAnimator->SetAnimation(static_cast<UINT>(m_State));
		}
	}

	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump) && m_TimesJumped < m_MaxPossibleJumps)
	{
		m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
		if (m_State != Jump)
		{
			m_State = Jump;
		}
		m_pModelAnimator->SetAnimation(static_cast<UINT>(m_State));
		auto soundManager = SoundManager::Get()->GetSystem();
		auto soundToPlay = m_TimesJumped % 2 == 0 ? m_pWooHooSound : m_pHooSound;
		bool isPlaying = false;
		m_pChannel3D->isPlaying(&isPlaying);
		if (isPlaying)m_pChannel3D->stop();
		soundManager->playSound(soundToPlay, nullptr, false, &m_pChannel3D);
		++m_TimesJumped;
	}

	//## Vertical Movement (Jump/Fall)
	//If the Controller Component is NOT grounded (= freefall)
		//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
		//Make sure that the minimum speed stays above -CharacterDesc::maxFallSpeed (negative!)
	//Else If the jump action is triggered
		//Set m_TotalVelocity.y equal to CharacterDesc::JumpSpeed
	//Else (=Character is grounded, no input pressed)
		//m_TotalVelocity.y is zero


	XMVECTOR velocityVec = XMLoadFloat3(&m_TotalVelocity);
	XMFLOAT3 displacement;
	XMStoreFloat3(&displacement, velocityVec * elapsedTime);
	m_pControllerComponent->Move(displacement);

	//************
	//DISPLACEMENT

	//The displacement required to move the Character Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
	//Calculate the displacement (m) for the current frame and move the ControllerComponent



	//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
	//Also, it can be usefull to use a seperate RayCast to check if the character is grounded (more responsive)

	auto playerSoundPos = ToFmod(GetTransform()->GetPosition());
	auto soundVel = ToFmod(m_TotalVelocity);
	m_pChannel3D->set3DAttributes(&playerSoundPos, &soundVel);
}

void Character::DrawImGui()
{
	if (ImGui::CollapsingHeader("Character"))
	{
		const auto pTransform = GetTransform();
		const auto pChildTransform = GetChildren<GameObject>()[0]->GetTransform();

		auto inputFlags = ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat3("Position", ConvertUtil::ToImFloatPtr(pTransform->GetPosition()), "%.2f", inputFlags);
		ImGui::InputFloat3("Forward", ConvertUtil::ToImFloatPtr(pChildTransform->GetForward()), "%.2f", inputFlags);

		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if (ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}
}