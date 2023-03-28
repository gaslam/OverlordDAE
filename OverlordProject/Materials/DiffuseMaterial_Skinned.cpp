#include "stdafx.h"
#include "DiffuseMaterial_Skinned.h"

DiffuseMaterial_Skinned::DiffuseMaterial_Skinned():
	Material(L"Effects/PosNormTex3D_Skinned.fx")
{
}

void DiffuseMaterial_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Skinned::InitializeEffectVariables()
{
}

void DiffuseMaterial_Skinned::OnUpdateModelVariables(const SceneContext&, const ModelComponent* modelComponent) const
{
	//Retrieve The Animator from the ModelComponent
	//Make sure the animator is not NULL (ASSERT_NULL_)
	//Retrieve the BoneTransforms from the Animator
	//Set the 'gBones' variable of the effect (MatrixArray) > BoneTransforms
	auto pAnimator = modelComponent->GetAnimator();
	ASSERT_NULL_(pAnimator);
	auto& boneTransforms = pAnimator->GetBoneTransforms();
	auto data = boneTransforms.data();
	const float* dataFl = reinterpret_cast<const float*>(data);
	SetVariable_MatrixArray(L"gBones",dataFl, static_cast<UINT>(boneTransforms.size()));
}
