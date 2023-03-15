#include "stdafx.h"
#include "W3_PongScene.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/CubePrefab.h"

W3_PongScene::W3_PongScene() : GameScene(L"PongScene")
{
}

void W3_PongScene::Initialize()
{
	auto& physx = PxGetPhysics();
	auto pBouncyMaterial = physx.createMaterial(.5f, .5f, 0.9f);

	const auto pFixedCamera = new FixedCamera();
	pFixedCamera->GetTransform()->Translate(0, 5.f, -20);
	AddChild(pFixedCamera);
	SetActiveCamera(pFixedCamera->GetComponent<CameraComponent>());

	const XMFLOAT4 blackColor{ 0.f,0.f,0.f,1.f };
	m_SceneContext.settings.clearColor = blackColor;

	InitSphere(pBouncyMaterial);
	InitCubes(pBouncyMaterial);
	InitGroundAndFloor(pBouncyMaterial);
	InitWalls(pBouncyMaterial);
	AddInput();

}

void W3_PongScene::Update()
{
	const float deltaTime{ m_SceneContext.pGameTime->GetElapsed() };
	m_AccTimeToRest += deltaTime;
	if (m_AccTimeToRest < m_TotalTimeToRest)
	{
		return;
	}

	auto ballPos = m_pRedSphere->GetTransform()->GetPosition();
	ballPos.x += (m_BallSpeed * deltaTime) * m_BallDir.x;
	ballPos.y += (m_BallSpeed * deltaTime) * m_BallDir.y;

	m_pRedSphere->GetTransform()->Translate(ballPos);

	if (m_SceneContext.pInput->IsActionTriggered(MovementInputPlayerOne::p1Up))
	{
		MoveCube(m_pCubes[0], m_CubeSpeed * deltaTime);
	}

	if (m_SceneContext.pInput->IsActionTriggered(MovementInputPlayerOne::p1Down))
	{
		MoveCube(m_pCubes[0], -m_CubeSpeed * deltaTime);;
	}

	if (m_SceneContext.pInput->IsActionTriggered(MovementInputPlayerTwo::p2Up))
	{
		MoveCube(m_pCubes[1], m_CubeSpeed * deltaTime);
	}

	if (m_SceneContext.pInput->IsActionTriggered(MovementInputPlayerTwo::p2Down))
	{
		MoveCube(m_pCubes[1], -m_CubeSpeed * deltaTime);
	}
}

void W3_PongScene::Draw()
{
}

void W3_PongScene::OnGUI()
{
}

void W3_PongScene::ChangeDirection(const Direction& direction)
{
	if (direction == Direction::UpDown)
	{
		m_BallDir.y = -m_BallDir.y;
	}

	if (direction == Direction::LeftRight)
	{
		m_BallDir.x = -m_BallDir.x;
	}
}

void W3_PongScene::ResetBall()
{
	m_pRedSphere->GetTransform()->Translate(0.f, 5.f, 0.f);
	m_AccTimeToRest = 0.f;
}

void W3_PongScene::InitSphere(PxMaterial* mat)
{
	m_pRedSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Red });
	AddChild(m_pRedSphere);

	auto pRigidBody = m_pRedSphere->AddComponent(new RigidBodyComponent());
	m_pRedSphere->GetTransform()->Translate(0.f, 5.f, 0.f);
	pRigidBody->SetConstraint(RigidBodyConstraint::All, false);
	pRigidBody->AddCollider(PxSphereGeometry{ 1.f }, *mat);
}

void W3_PongScene::InitCubes(PxMaterial* mat)
{
	XMFLOAT3 startPos{ -10.f,5.f,0.f };

	for (int i{}; i < m_TotalCollObjsPerType; ++i)
	{
		m_pCubes[i] = new CubePrefab{ 1.f,3.f,1.f,XMFLOAT4{Colors::White} };
		AddChild(m_pCubes[i]);
		auto pRigidBody = m_pCubes[i]->AddComponent(new RigidBodyComponent(true));
		auto colliderId = pRigidBody->AddCollider(PxBoxGeometry{ .5f,1.5f,.5f }, *mat);
		auto colliderInfo = pRigidBody->GetCollider(colliderId);
		colliderInfo.SetTrigger(true);
	}

	m_pCubes[0]->GetTransform()->Translate(startPos);
	startPos.x = -startPos.x;
	m_pCubes[1]->GetTransform()->Translate(startPos);

	m_pCubes[0]->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (pTrigger == m_pCubes[0] && pOther == m_pRedSphere && action == PxTriggerAction::ENTER)
			{
				const Direction cubeDir{ Direction::LeftRight };
				ChangeDirection(cubeDir);
			}
		});

	m_pCubes[1]->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (pTrigger == m_pCubes[1] && pOther == m_pRedSphere && action == PxTriggerAction::ENTER)
			{
				const Direction cubeDir{ Direction::LeftRight };
				ChangeDirection(cubeDir);
			}
		});
}

void W3_PongScene::InitGroundAndFloor(PxMaterial* mat)
{
	XMFLOAT3 startPos{ 0.f,-3.3f,0.f };
	m_CubeMinHeight = startPos.y + .2f;
	for (int i{}; i < m_TotalCollObjsPerType; ++i)
	{
		m_pFloorAndRoof[i] = new CubePrefab{ 0.f,0.f,0.f,XMFLOAT4{Colors::White} };
		AddChild(m_pFloorAndRoof[i]);
		auto pRigidBody = m_pFloorAndRoof[i]->AddComponent(new RigidBodyComponent(true));
		auto colliderId = pRigidBody->AddCollider(PxBoxGeometry{ 20.f,.2f,1.f }, *mat);
		auto colliderInfo = pRigidBody->GetCollider(colliderId);
		colliderInfo.SetTrigger(true);
	}

	m_pFloorAndRoof[0]->GetTransform()->Translate(startPos);
	startPos.y = 13.5f;
	m_CubeMaxHeight = startPos.y + .2f;
	m_pFloorAndRoof[1]->GetTransform()->Translate(startPos);

	m_pFloorAndRoof[0]->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (pTrigger == m_pFloorAndRoof[0] && pOther == m_pRedSphere && action == PxTriggerAction::ENTER)
			{
				const Direction cubeDir{ Direction::UpDown };
				ChangeDirection(cubeDir);
			}
		});

	m_pFloorAndRoof[1]->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (pTrigger == m_pFloorAndRoof[1] && pOther == m_pRedSphere && action == PxTriggerAction::ENTER)
			{
				const Direction cubeDir{ Direction::UpDown };
				ChangeDirection(cubeDir);
			}
		});
}

void W3_PongScene::InitWalls(PxMaterial* mat)
{
	XMFLOAT3 startPos{ -15.f,-0.f,0.f };
	for (int i{}; i < m_TotalCollObjsPerType; ++i)
	{
		m_pWalls[i] = new CubePrefab{ 0.f,0.f,0.f,XMFLOAT4{Colors::White} };
		AddChild(m_pWalls[i]);
		auto pRigidBody = m_pWalls[i]->AddComponent(new RigidBodyComponent(true));
		auto colliderId = pRigidBody->AddCollider(PxBoxGeometry{ .2f,20.f,1.f }, *mat);
		auto colliderInfo = pRigidBody->GetCollider(colliderId);
		colliderInfo.SetTrigger(true);
	}

	m_pWalls[0]->GetTransform()->Translate(startPos);
	startPos.x = 15.f;
	m_pWalls[1]->GetTransform()->Translate(startPos);

	m_pWalls[0]->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (pTrigger == m_pWalls[0] && pOther == m_pRedSphere && action == PxTriggerAction::ENTER)
			{
				ResetBall();
			}
		});

	m_pWalls[1]->SetOnTriggerCallBack([=](GameObject* pTrigger, GameObject* pOther, PxTriggerAction action)
		{
			if (pTrigger == m_pWalls[1] && pOther == m_pRedSphere && action == PxTriggerAction::ENTER)
			{
				ResetBall();
			}
		});
}

void W3_PongScene::AddInput()
{
	m_SceneContext.pInput->AddInputAction(InputAction{ MovementInputPlayerOne::p1Up,InputState::down,'W',-1,XINPUT_GAMEPAD_DPAD_UP,GamepadIndex::playerTwo });
	m_SceneContext.pInput->AddInputAction(InputAction{ MovementInputPlayerOne::p1Down,InputState::down,'S',-1,XINPUT_GAMEPAD_DPAD_DOWN,GamepadIndex::playerTwo });
	m_SceneContext.pInput->AddInputAction(InputAction{ MovementInputPlayerTwo::p2Up,InputState::down,VK_UP,-1,XINPUT_GAMEPAD_DPAD_UP,GamepadIndex::playerOne });
	m_SceneContext.pInput->AddInputAction(InputAction{ MovementInputPlayerTwo::p2Down,InputState::down,VK_DOWN,-1,XINPUT_GAMEPAD_DPAD_DOWN,GamepadIndex::playerOne });
}

void W3_PongScene::MoveCube(CubePrefab* prefab, float distance)
{
	auto cubePos = prefab->GetTransform()->GetPosition();
	cubePos.y += distance;

	auto pRigidBody = prefab->GetComponent<RigidBodyComponent>();
	auto cubeHeight = pRigidBody->GetCollider(0).GetShape()->getGeometry().box().halfExtents.y;

	if (cubePos.y <= m_CubeMinHeight)
	{
		cubePos.y = m_CubeMinHeight;
	}

	if (!pRigidBody)
	{
		return;
	}
	auto roofPos = m_pFloorAndRoof[1]->GetTransform()->GetPosition();
	if (cubePos.y + cubeHeight >= m_CubeMaxHeight)
	{
		cubePos.y = m_CubeMaxHeight - cubeHeight;
	}
	prefab->GetTransform()->Translate(cubePos);
}
