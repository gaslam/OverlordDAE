#include "stdafx.h"
#include "BanjoTestScene.h"
#include <Materials/DiffuseMaterial.h>
#include "Prefabs/ThirdPersonCamera.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Blueprints/PlayableCharacter/PlayableCharacter.h"

void BanjoTestScene::Initialize()
{
	const auto banjo = AddChild(new PlayableCharacter{});

	auto pTransform = banjo->GetComponent<TransformComponent>();
	pTransform->Translate(-10.f, 0.f, 0.f);

	const auto p3rdPersonCam = banjo->GetCamera();
	AddChild(p3rdPersonCam);
	SetActiveCamera(p3rdPersonCam->GetComponent<CameraComponent>());
}

void BanjoTestScene::OnGUI()
{
}
