#include "stdafx.h"
#include "ModelTestScene.h"
#include <Materials/ColorMaterial.h>
#include <Materials/DiffuseMaterial.h>

ModelTestScene::ModelTestScene() : GameScene(L"ModelTestScene")
{
}

void ModelTestScene::Initialize()
{
	auto& physx = PxGetPhysics();
	auto physxMat = physx.createMaterial(1.f, 1.f, 0.1f);
	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Chair.ovm"} };
	m_pChair = new GameObject{};
	m_pChair->AddComponent<ModelComponent>(modelComponent);
	AddChild(m_pChair);

	//auto mat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	//mat->SetColor(XMFLOAT4{ Colors::Red });

	auto pDiffuseMat = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pDiffuseMat->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	modelComponent->SetMaterial(pDiffuseMat);

	auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
	auto pRigidBody = m_pChair->AddComponent<RigidBodyComponent>(new RigidBodyComponent());
	pRigidBody->AddCollider(PxConvexMeshGeometry{pConvexMesh}, *physxMat);

	m_pChair->GetTransform()->Translate(XMVECTOR{ 0.f,10.f,0.f });

	GameSceneExt::CreatePhysXGroundPlane(*this, physxMat);
}

void ModelTestScene::Update()
{
}

void ModelTestScene::Draw()
{
}

void ModelTestScene::OnGUI()
{
}
