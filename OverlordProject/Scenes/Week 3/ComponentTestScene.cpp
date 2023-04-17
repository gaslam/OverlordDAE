#include "stdafx.h"
#include "ComponentTestScene.h"
#include "Prefabs/SpherePrefab.h"


ComponentTestScene::ComponentTestScene() :
	GameScene(L"ComponentTestScene") {}

void ComponentTestScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	Logger::LogInfo(L"Welcome, humble component tester!");

	auto& physx = PxGetPhysics();
	auto pBouncyMaterial = physx.createMaterial(.5f, .5f, 0.9f);

	//Ground plane
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	//Red sphere (group 0)
	m_pRedSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Red });

	AddChild(m_pRedSphere);
	auto pRigidBody = m_pRedSphere->AddComponent(new RigidBodyComponent());
	pRigidBody->AddCollider(PxSphereGeometry{ 1.f },*pBouncyMaterial);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);
	//pRigidBody->SetCollisionIgnoreGroups(CollisionGroup::Group1);
	//pRigidBody->SetConstraint(RigidBodyConstraint::TransX | RigidBodyConstraint::TransZ,false);

	//Green sphere (group 1)
	auto pGreenSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Green });

	AddChild(pGreenSphere);
	pGreenSphere->GetTransform()->Translate(0, 20.f, 0.f);
	pRigidBody = pGreenSphere->AddComponent(new RigidBodyComponent());
	pRigidBody->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);

	//Blue sphere (group 2)
	auto pBlueSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Blue });

	AddChild(pBlueSphere);
	pBlueSphere->GetTransform()->Translate(0, 10.f, 0);
	pRigidBody = pBlueSphere->AddComponent(new RigidBodyComponent());
	pRigidBody->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group2);
	pRigidBody->AddForce({ 0,-100,0 }, PxForceMode::eIMPULSE);

	m_SceneContext.pInput->AddInputAction(InputAction{ Inputs::reset,InputState::pressed,'R' });

}

void ComponentTestScene::Update()
{
}

void ComponentTestScene::Draw()
{
	//Optional
}

void ComponentTestScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}

