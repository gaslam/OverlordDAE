#pragma once
#include <type_traits>
#include <stdafx.h>
#include "Misc/BaseMaterial.h"

namespace ModelUtils {
	template <typename T>
	std::enable_if<std::is_base_of_v<BaseMaterial, T>>::type
		AddTexturesToModelComponent(ModelComponent* pModelComp, std::wstring& texturePath, std::wstring& extension) {

		if (!pModelComp)
		{
			Logger::LogWarning(L"ModelComponent is nullptr!!");
			return;
		}

		if (!pModelComp->HasMeshFilter())
		{
			Logger::LogWarning(L"ModelComponent has no mesh filter!!");
			return;
		}

		for (auto& mesh : pModelComp->GetMeshFilter()->GetMeshes())
		{
			std::wstring assetFile = texturePath + mesh.name + L"_diffuse." + extension;
			const auto fullPath = ContentManager::GetFullAssetPath(assetFile);
			if (fs::exists(fullPath))
			{
				const auto pMaterial = MaterialManager::Get()->CreateMaterial<T>();
				pModelComp->SetMaterial(pMaterial, mesh.id);
				pMaterial->SetDiffuseTexture(assetFile);
			}
		}
	}

	template <typename T>
	std::enable_if<std::is_base_of_v<BaseMaterial, T>>::type
		AddTexturesToModelComponent(ModelComponent* pModelComp, std::wstring& texturePath, std::vector<std::wstring>& textures, std::wstring& extension) {

		if (!pModelComp)
		{
			Logger::LogWarning(L"ModelComponent is nullptr!!");
			return;
		}

		if (!pModelComp->HasMeshFilter())
		{
			Logger::LogWarning(L"ModelComponent has no mesh filter!!");
			return;
		}

		auto meshes = pModelComp->GetMeshFilter()->GetMeshes();

		for (size_t i{}; i < textures.size(); ++i)
		{
			auto mesh = meshes[i];
			std::wstring assetFile = texturePath + textures[i] + L"_diffuse." + extension;
			const auto fullPath = ContentManager::GetFullAssetPath(assetFile);
			if (fs::exists(fullPath))
			{
				const auto pMaterial = MaterialManager::Get()->CreateMaterial<T>();
				pModelComp->SetMaterial(pMaterial, mesh.id);
				pMaterial->SetDiffuseTexture(assetFile);
			}
		}
	}
}