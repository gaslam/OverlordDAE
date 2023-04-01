#include "stdafx.h"
#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(TransformComponent* pComponent, float distance) : m_pComponent{ pComponent }, m_Distance{ distance } {

};

void ThirdPersonCamera::Initialize(const SceneContext&)
{
	m_pCamera = AddComponent(new CameraComponent());
}

void ThirdPersonCamera::Update(const SceneContext& sceneContext)
{
	if (m_pCamera->IsActive())
	{
		auto mouseDir = sceneContext.pInput->GetMouseMovementNormalized();
		XMFLOAT2 look{ 0, 0 };
		bool mouseMoved{ false };
		const auto& mouseMove = InputManager::GetMouseMovement();
		look.x = static_cast<float>(mouseMove.x);
		look.y = static_cast<float>(mouseMove.y);

		mouseMoved = mouseMove.x != 0 || mouseMove.y != 0;
		if (!mouseMoved)
		{
			look = InputManager::GetThumbstickPosition(false);
		}
		const float elapsedTime = sceneContext.pGameTime->GetElapsed();

		m_TotalYaw += -look.x * m_RotSpeed * elapsedTime;
		m_TotalPitch += -look.y * m_RotSpeed * elapsedTime;

		if (m_TotalPitch > m_MaxAngle)
		{
			m_TotalPitch = m_MaxAngle;
		}
		if (m_TotalPitch < m_MinAngle)
		{
			m_TotalPitch = m_MinAngle;
		}

		const auto backward = -(XMLoadFloat3(&GetTransform()->GetForward()) * m_Distance);
		auto currPos = XMLoadFloat3(&m_pComponent->GetTransform()->GetPosition()) + backward;
		GetTransform()->Translate(currPos);
		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0.f);
	}
}
