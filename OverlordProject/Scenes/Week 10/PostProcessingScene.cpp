#include "stdafx.h"
#include "PostProcessingScene.h"

//#define SHADOW
#define DIFFUSE
//Uncomment for shadow variants

#ifdef SHADOW
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#endif

#ifdef DIFFUSE
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#endif


#include "Materials/Post/PostGrayscale.h"
#include "Materials/Post/PostBlur.h"

void PostProcessingScene::Initialize()
{
	m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	//Materials
	//*********
#ifdef SHADOW
	const auto pPeasantMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>(); //Shadow variant
	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>(); //Shadow variant
#endif
#ifdef DIFFUSE
	const auto pPeasantMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	const auto pGroundMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
#endif

	pPeasantMaterial->SetDiffuseTexture(L"Textures/PeasantGirl_Diffuse.png");

	pGroundMaterial->SetDiffuseTexture(L"Textures/GroundBrick.jpg");

	//Ground Mesh
	//***********
	const auto pGroundObj = new GameObject();
	const auto pGroundModel = new ModelComponent(L"Meshes/UnitPlane.ovm");
	pGroundModel->SetMaterial(pGroundMaterial);

	pGroundObj->AddComponent(pGroundModel);
	pGroundObj->GetTransform()->Scale(10.0f, 10.0f, 10.0f);

	AddChild(pGroundObj);

	//Character Mesh
	//**************
	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/PeasantGirl.ovm"));
	pModel->SetMaterial(pPeasantMaterial);

	pObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	if (const auto pAnimator = pModel->GetAnimator())
	{
		pAnimator->SetAnimation(2);
		pAnimator->Play();
	}

	//Post Processing Stack
	//=====================
	m_pPostGrayscale = MaterialManager::Get()->CreateMaterial<PostGrayscale>();
	m_pPostBlur = MaterialManager::Get()->CreateMaterial<PostBlur>();

	AddPostProcessingEffect(m_pPostGrayscale);
	AddPostProcessingEffect(m_pPostBlur);

	//TODO_W10
	//Create and add a PostBlur material class (using Blur.fx)
	//Add the material to the PostProcessing Stack + Uncomment the corresponding ImGui code below
	//If you already have shadow mapping working you can change the code above to use the shadow variant shaders
}

void PostProcessingScene::OnGUI()
{
	bool isEnabled = m_pPostGrayscale->IsEnabled();
	ImGui::Checkbox("Grayscale PP", &isEnabled);
	m_pPostGrayscale->SetIsEnabled(isEnabled);

	isEnabled = m_pPostBlur->IsEnabled();
	ImGui::Checkbox("Blur PP", &isEnabled);
	m_pPostBlur->SetIsEnabled(isEnabled);
}