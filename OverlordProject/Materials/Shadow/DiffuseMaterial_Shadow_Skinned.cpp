#include "stdafx.h"
#include "DiffuseMaterial_Shadow_Skinned.h"

DiffuseMaterial_Shadow_Skinned::DiffuseMaterial_Shadow_Skinned():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow_Skinned.fx")
{}

void DiffuseMaterial_Shadow_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	/*
	 * TODO_W8
	 * Update The Shader Variables
	 * 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	 * 	LightWVP = model_world * light_viewprojection
	 * 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	 * 
	 * 2. Update the ShadowMap texture
	 * 
	 * 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	 * 
	 * 4. Update Bones
	*/

	auto lightWorldVP = XMLoadFloat4x4(&pModel->GetTransform()->GetWorld()) * XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP());
	XMFLOAT4X4 lightWorldVPFl;

	XMStoreFloat4x4(&lightWorldVPFl, lightWorldVP);

	SetVariable_Matrix(L"gWorldViewProj_Light", reinterpret_cast<const float*>(&lightWorldVPFl));

	SetVariable_Texture(L"gShadowMap", ShadowMapRenderer::Get()->GetShadowMap());
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	auto pAnimator = pModel->GetAnimator();
	ASSERT_IF(!pModel->HasAnimator(),L"model has no animator!!");
	auto boneTransforms = pAnimator->GetBoneTransforms();
	SetVariable_MatrixArray(L"gBones", reinterpret_cast<const float*>(boneTransforms.data()), static_cast<UINT>(boneTransforms.size()));
}
