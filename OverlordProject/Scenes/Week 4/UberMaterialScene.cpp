#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Materials/UberMaterial.h"

UberMaterialScene::UberMaterialScene() : GameScene(L"UberMaterialScene")
{
}

void UberMaterialScene::Initialize()
{
	m_pSphere = new GameObject{};
	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Sphere.ovm"} };
	m_pSphere->AddComponent(modelComponent);
	auto pUberMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pUberMat->SetUseDiffuseTexture(true);
	pUberMat->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	m_UberMatId = pUberMat->GetMaterialId();

	XMFLOAT4 colorRed{ XMFLOAT4{ Colors::Red } };
	XMFLOAT4 colorBlack{ XMFLOAT4{ Colors::Black } };
	XMFLOAT4 colorWhite{ XMFLOAT4{ Colors::White } };
	int shininess{ 100 };
	float reflectionStrength{ 0.16f };
	float fresnelMultiplier{ 1.f };
	float opacityIntensity{ 1.f };
	pUberMat->SetColorDiffuse(colorRed);
	pUberMat->SetColorSpecular(colorBlack);
	pUberMat->SetShininess(shininess);
	pUberMat->SetColorAmbient(colorRed);
	pUberMat->SetFlipGreenChannel(false);
	pUberMat->SetUseTextureNormal(true);
	pUberMat->SetUseEnvMapping(true);
	pUberMat->SetReflectionStrength(reflectionStrength);
	pUberMat->SetColorFresnel(colorWhite);
	pUberMat->SetFresnelMultiplier(fresnelMultiplier);
	pUberMat->SetOpacityIntensity(opacityIntensity);
	pUberMat->SetUseSpecularPhong(true);
	pUberMat->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	pUberMat->SetOpacityMapTexture(L"Textures/Skulls_Heightmap.tga");

	modelComponent->SetMaterial(pUberMat);

	AddChild(m_pSphere);
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	auto pCamTransform = m_SceneContext.pCamera->GetTransform();
	pCamTransform->Translate(0.f, 4.f, -6.f);
	
}

void UberMaterialScene::Update()
{
}

void UberMaterialScene::Draw()
{
}

void UberMaterialScene::OnGUI()
{
	auto pUberMat = MaterialManager::Get()->GetMaterial<UberMaterial>(m_UberMatId);
	if (!pUberMat)
	{
		return;
	}

	const std::string buttonText{ "Material ID #" + std::to_string(m_UberMatId) };
	if (ImGui::Button(buttonText.c_str()))
	{
		m_MaterialPopupWindowOpwn = true;
	}

	if (m_MaterialPopupWindowOpwn)
	{
		if (ImGui::Begin("Dear ImGui Demo", &m_MaterialPopupWindowOpwn))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::PushFont(ImguiFonts::pFont_DIN_Black_18);
			ImGui::Text("Diffuse");
			ImGui::PopFont();

			bool useTextureDiffuse = pUberMat->GetUseTextureDiffuse();
			ImGui::Checkbox("Use Texure Diffuse", &useTextureDiffuse);
			pUberMat->SetUseDiffuseTexture(useTextureDiffuse);

			XMFLOAT4 diffuseColor = pUberMat->GetDiffuseColor();

			ImGuiColorEditFlags colorFlags = ImGuiColorEditFlags_NoInputs;

			ImGui::ColorEdit4("Diffuse Color", ConvertUtil::ToImFloatPtr(diffuseColor),colorFlags);
			pUberMat->SetColorDiffuse(diffuseColor);

			ImGui::NewLine();
			ImGui::PushFont(ImguiFonts::pFont_DIN_Black_18);
			ImGui::Text("Opacity");
			ImGui::PopFont();

			float opacityIntensity = pUberMat->GetOpacityIntensity();
			ImGui::SliderFloat("Opacity Intensity", &opacityIntensity, 0.f, 1.f);
			pUberMat->SetOpacityIntensity(opacityIntensity);

			bool useOpacityIntensity = pUberMat->GetUseOpacityIntensity();
			ImGui::Checkbox("Use opacity intensity map", &useOpacityIntensity);
			pUberMat->SetUseOpacityIntensity(useOpacityIntensity);

			ImGui::NewLine();

			ImGui::PushFont(ImguiFonts::pFont_DIN_Black_18);
			ImGui::Text("Specular");
			ImGui::PopFont();


			bool useSpecularIntensity = pUberMat->GetUseSpecularIntensity();
			ImGui::Checkbox("Use specular intensity", &useSpecularIntensity);
			pUberMat->SetUseSpecularIntensity(useSpecularIntensity);

			XMFLOAT4 specularColor = pUberMat->GetSpecularColor();

			ImGui::ColorEdit4("Specular Color", ConvertUtil::ToImFloatPtr(specularColor),colorFlags);
			pUberMat->SetColorSpecular(specularColor);

			int shininess = pUberMat->GetShininess();
			ImGui::SliderInt("Shininess", &shininess, 1, 100);
			pUberMat->SetShininess(shininess);

			ImGui::NewLine();
			ImGui::PushFont(ImguiFonts::pFont_DIN_Black_18);
			ImGui::Text("Selected Technique");
			ImGui::PopFont();


			auto names{ pUberMat->GetTechniqueDesciptions() };
			int namesSize = static_cast<int>(names.size());
			const char** newNames{ new const char* [namesSize]{} };
			for (auto name : names)
			{
				newNames[name.first] = name.second;
			}
			int previousItem = m_SelectedTechnique;
			if (ImGui::CollapsingHeader(newNames[m_SelectedTechnique]))
			{
				ImGui::ListBox("##listboxTechniques", &m_SelectedTechnique, newNames, namesSize);
				if (m_SelectedTechnique != previousItem)
				{
					pUberMat->SetTechnique(m_SelectedTechnique);
				}
			}
			ImGui::End();
			return;
		}
	}
}
