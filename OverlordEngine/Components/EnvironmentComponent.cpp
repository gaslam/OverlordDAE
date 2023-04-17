#include "stdafx.h"
#include "EnvironmentComponent.h"

EnvironmentComponent::EnvironmentComponent(PlayableCharacter* pCharacter) : m_pPlayableCharacter{ pCharacter }
{
}

void EnvironmentComponent::Update(const SceneContext& context)
{
	//context.pCamera->GetTransform().GetF
	//for (auto& environmentModel : m_Environments)
	//{
	//	if (!environmentModel->HasMeshFilter())
	//	{
	//		continue;
	//	}
	//	auto pMeshFilter = environmentModel->GetMeshFilter();
	//	for (auto& mesh : pMeshFilter->GetMeshes())
	//	{
	//		auto& positions = mesh.positions;
	//		bool isHit{ false };
	//		for (int i{}; i < positions.size() - 2; ++i)
	//		{
	//			XMVECTOR triangleP1{ XMLoadFloat3(&positions[i]) };
	//			XMVECTOR triangleP2{ XMLoadFloat3(&positions[i + 1]) };
	//			XMVECTOR triangleP3{ XMLoadFloat3(&positions[i + 2]) };
	//			XMVECTOR dir{ XMVector3Normalize(backward) };
	//			bool test = TriangleTests::Intersects(currPos, dir, triangleP1, triangleP2, triangleP3, dist);
	//			if (test && dist < 50)
	//			{
	//				std::cout << "intersection hit. dist: " << dist << "\n";
	//				backward = -(XMLoadFloat3(&GetTransform()->GetForward()) * dist);
	//				currPos = XMLoadFloat3(&m_pComponent->GetTransform()->GetPosition()) + backward;
	//				isHit = true;
	//				break;
	//			}
	//		}
	//		if (isHit)
	//		{
	//			break;
	//		}
	//	}
	//}
}
