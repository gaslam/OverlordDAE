#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "Misc/MeshFilter.h"

ThirdPersonCamera::ThirdPersonCamera(float distance) : m_Distance{ distance } {

};

void ThirdPersonCamera::AddModelToAvoid(ModelComponent* modelComponent)
{
	m_pModelsToAvoid.push_back(modelComponent);
}

void ThirdPersonCamera::RemoveModelToAvoid(ModelComponent* modelComponent)
{
	auto removedModel = std::remove_if(m_pModelsToAvoid.begin(), m_pModelsToAvoid.end(), [modelComponent](ModelComponent* component) {
		return component == modelComponent;
		});
	if (removedModel != m_pModelsToAvoid.end())
	{
		m_pModelsToAvoid.erase(removedModel);
	}
}

void ThirdPersonCamera::Initialize(const SceneContext&)
{
	m_pCamera = AddComponent(new CameraComponent());
}

void ThirdPersonCamera::Update(const SceneContext& sceneContext)
{
	if (m_pCamera->IsActive())
	{
		XMFLOAT2 look{ 0, 0 };
		const auto& mouseMove = InputManager::GetMouseMovement();
		look.x = static_cast<float>(mouseMove.x);
		look.y = static_cast<float>(mouseMove.y);
		float rotSpeed{ m_MouseRotSpeed };

		bool mouseMoved = mouseMove.x != 0 || mouseMove.y != 0;
		if (!mouseMoved)
		{
			look = InputManager::GetThumbstickPosition(false);
			rotSpeed = m_ControllerRotSpeed;
		}
		const float elapsedTime = sceneContext.pGameTime->GetElapsed();
		m_TotalYaw += -look.x * rotSpeed * elapsedTime;
		m_TotalPitch += -look.y * rotSpeed * elapsedTime;

		if (m_TotalPitch > m_MaxAngle)
		{
			m_TotalPitch = m_MaxAngle;
		}
		if (m_TotalPitch < m_MinAngle)
		{
			m_TotalPitch = m_MinAngle;
		}

		if (m_TotalYaw >= 360.f)
		{
			m_TotalYaw = 0.f;
		}
		auto backwards = -(XMLoadFloat3(&GetTransform()->GetForward()));
		auto pos = GetParent()->GetTransform()->GetPosition();
		auto currPos = XMLoadFloat3(&pos);
		float oldDist{ FLT_MAX };
		for (const auto environmentModel : m_pModelsToAvoid)
		{
			if (!environmentModel->HasMeshFilter())
			{
				continue;
			}
			auto pMeshFilter = environmentModel->GetMeshFilter();
			for (auto& mesh : pMeshFilter->GetMeshes())
			{
				auto& positions = mesh.positions;
				float dist{};
				for (int i{}; i < positions.size() - 2; ++i)
				{
					XMVECTOR triangleP1{ XMLoadFloat3(&positions[i]) };
					XMVECTOR triangleP2{ XMLoadFloat3(&positions[++i]) };
					XMVECTOR triangleP3{ XMLoadFloat3(&positions[++i]) };
					bool test = TriangleTests::Intersects(currPos, backwards, triangleP1, triangleP2, triangleP3, dist);
					if (test && dist < 10.f && dist < oldDist)
					{
						std::cout << "intersection hit. dist: " << dist << "\n";
						oldDist = dist;
						if (oldDist < 5.f)
						{
							if (oldDist < 1.f)
							{
								oldDist = 0.1f;
							}
							else
							{
								oldDist = 1.f;
							}
						}
						break;
					}
				}
			}
		}
		if (oldDist == FLT_MAX)
		{
			oldDist = m_Distance;
		}
		auto finalBackwardsDir = backwards * oldDist;
		GetTransform()->Translate(finalBackwardsDir);
		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0.f);
		XMStoreFloat3(&m_BackwardsDir, finalBackwardsDir);
	}
}

void ThirdPersonCamera::Draw(const SceneContext&)
{
	auto start = GetParent()->GetTransform()->GetPosition();
	start.y += 0.001f;
	auto end = XMLoadFloat3(&start) + (XMLoadFloat3(&m_BackwardsDir) * 1.f);
	XMFLOAT3 endFl;
	XMStoreFloat3(&endFl, end);
	endFl.z -= 0.001f;
	DebugRenderer::DrawLine(start, endFl);
}

void ThirdPersonCamera::Intersect(MeshFilter*)
{
}
