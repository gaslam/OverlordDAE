#include "stdafx.h"
#include "ModelTestScene.h"
#include <Materials/DiffuseMaterial.h>

ModelTestScene::ModelTestScene() : GameScene(L"ModelTestScene")
{
}

void ModelTestScene::Initialize()
{
	auto& physx = PxGetPhysics();
	auto physxMat = physx.createMaterial(1.f, 1.f, 0.1f);
	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Objects/Chair.ovm"} };
	m_pChair = new GameObject{};
	m_pChair->AddComponent<ModelComponent>(modelComponent);
	AddChild(m_pChair);

	//auto mat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	//mat->SetColor(XMFLOAT4{ Colors::Red });

	auto pDiffuseMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pDiffuseMat->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	modelComponent->SetMaterial(pDiffuseMat);

	auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Objects/Chair.ovpc");
	auto pRigidBody = m_pChair->AddComponent<RigidBodyComponent>(new RigidBodyComponent());
	pRigidBody->AddCollider(PxConvexMeshGeometry{pConvexMesh}, *physxMat);

	LoadScene();
	GameSceneExt::CreatePhysXGroundPlane(*this, physxMat);
	m_SceneContext.pInput->AddInputAction(InputAction{ Inputs::reset,InputState::pressed,'R' });
}

void ModelTestScene::Update()
{
	if (m_SceneContext.pInput->IsActionTriggered(Inputs::reset))
	{
		LoadScene();
	}
}

void ModelTestScene::Draw()
{
}

void ModelTestScene::OnGUI()
{
}

void ModelTestScene::LoadScene()
{
	m_pChair->GetTransform()->Translate(XMVECTOR{ 0.f,10.f,0.f });
	auto pGameObject = m_SceneContext.pCamera->GetGameObject();
	if (!pGameObject)
	{
		Logger::LogError(L"Camera GameObject not found!!");
	}

	auto pCamera = dynamic_cast<FreeCamera*>(pGameObject);
	if (!pCamera)
	{
		Logger::LogError(L"Camera not found!!");
	}
	pCamera->SetRotation(30.f, 0.f);
	auto pTransform = pCamera->GetTransform();
	if (!pTransform)
	{
		Logger::LogError(L"Camera transform not found!!");
		return;
	}
	pTransform->GetTransform()->Translate(0, 50, -80);
}

void ModelTestScene::OnSceneActivated()
{
	LoadScene();
}
