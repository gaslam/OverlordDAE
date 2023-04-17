#include "stdafx.h"
#include "BanjoTestScene.h"
#include <Materials/DiffuseMaterial.h>
#include "Prefabs/ThirdPersonCamera.h"
#include "Materials/DiffuseMaterial.h"
#include "Blueprints/PlayableCharacter/PlayableCharacter.h"
#include <Materials/ColorMaterial.h>
#include "ProjectUtils.h"
#include <Materials/DiffuseMaterial_Skinned.h>

void BanjoTestScene::Initialize()
{
	m_pPlayableCharacter = AddChild(new PlayableCharacter{});

	auto pTransform = m_pPlayableCharacter->GetComponent<TransformComponent>();
	//pTransform->Translate(75.f, 0.f, 250.f);
	pTransform->Scale(1.2f);

	const auto p3rdPersonCam = m_pPlayableCharacter->GetCamera();
	p3rdPersonCam->SetCameraDistance(50.f);
	AddChild(p3rdPersonCam);

	std::wstring playerTexturePath = std::wstring{ L"Textures/Characters/Banjo/" };
	std::wstring textureExtension = L"png";

	ModelUtils::AddTexturesToModelComponent<DiffuseMaterial_Skinned>(m_pPlayableCharacter->GetComponent<ModelComponent>(), playerTexturePath, textureExtension);
	

	SetActiveCamera(p3rdPersonCam->GetComponent<CameraComponent>());
	ModelComponent* modelComponentEnv = new ModelComponent{L"Meshes/Environments/EnvironmentTest2.ovm"};
	auto object = new GameObject{};
	object->AddComponent<ModelComponent>(modelComponentEnv);
	AddChild(object);

	std::wstring textureFolder = L"Textures/Environments/Click_Clock_Woods_Summer/";

	ModelUtils::AddTexturesToModelComponent<DiffuseMaterial>(modelComponentEnv,textureFolder, textureExtension);

	m_SceneContext.settings.enableOnGUI = true;

	auto& physx = PxGetPhysics();
	auto physxMat = physx.createMaterial(1.f, 10.f, 0.f);
	auto pTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Environments/EnvironmentTest2.ovpt");
	auto pRigidBody = object->AddComponent<RigidBodyComponent>(new RigidBodyComponent(true));
	pRigidBody->AddCollider(PxTriangleMeshGeometry{ pTriangleMesh }, *physxMat);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);

	pRigidBody = m_pPlayableCharacter->AddComponent<RigidBodyComponent>(new RigidBodyComponent());
	physxMat = physx.createMaterial(0.5f, 0.2f, 0.f);
	pRigidBody->AddCollider(PxSphereGeometry{5.f},*physxMat);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);
	pRigidBody->SetConstraint(RigidBodyConstraint::TransY, false);
}

void BanjoTestScene::OnGUI()
{
	if (m_pPlayableCharacter)
	{
		const auto pTransform = m_pPlayableCharacter->GetTransform();

		auto inputFlags = ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat3("Position", ConvertUtil::ToImFloatPtr(pTransform->GetPosition()),"%.2f", inputFlags);
	}
}
