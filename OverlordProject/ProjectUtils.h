#pragma once
#include <stdafx.h>

#include "Content/JsonLoader.h"
#include "Misc/json.hpp"
#include <string>
#include <algorithm>

#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterial_Skinned.h"
#include "Materials/OpacityMaterial.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

using json = nlohmann::json;

namespace ModelUtils {

	enum class MaterialType
	{
		diffuse,
		opacity,
		skinned,
		shadowSkinned,
		shadow
	};

	struct TextureInfo
	{
		std::wstring name;
		std::wstring extension;
		MaterialType type;
		UINT meshIdx;
	};

	inline DiffuseMaterial_Skinned* CreateDiffuseSkinnedMaterial(const std::wstring& texturePath, const TextureInfo& info)
	{
		const std::wstring assetDiffuseFile = texturePath + info.name + L"_diffuse." + info.extension;
		auto fullPath = ContentManager::GetFullAssetPath(assetDiffuseFile);
		DiffuseMaterial_Skinned* pOpMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
		if (fs::exists(fullPath))
		{
			pOpMaterial->SetDiffuseTexture(assetDiffuseFile);
		}

		return pOpMaterial;
	}

	inline DiffuseMaterial_Shadow_Skinned* CreateDiffuseShadowSkinnedMaterial(const std::wstring& texturePath, const TextureInfo& info)
	{
		const std::wstring assetDiffuseFile = texturePath + info.name + L"_diffuse." + info.extension;
		auto fullPath = ContentManager::GetFullAssetPath(assetDiffuseFile);
		DiffuseMaterial_Shadow_Skinned* pOpMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>();
		if (fs::exists(fullPath))
		{
			pOpMaterial->SetDiffuseTexture(assetDiffuseFile);
		}

		return pOpMaterial;
	}

	inline DiffuseMaterial_Shadow* CreateDiffuseShadowMaterial(const std::wstring& texturePath, const TextureInfo& info)
	{
		const std::wstring assetDiffuseFile = texturePath + info.name + L"_diffuse." + info.extension;
		auto fullPath = ContentManager::GetFullAssetPath(assetDiffuseFile);
		DiffuseMaterial_Shadow* pOpMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
		if (fs::exists(fullPath))
		{
			pOpMaterial->SetDiffuseTexture(assetDiffuseFile);
		}

		return pOpMaterial;
	}

	inline OpacityMaterial* CreateOpacityMaterial(const std::wstring& texturePath, const TextureInfo& info)
	{
		const std::wstring assetDiffuseFile = texturePath + info.name + L"_diffuse." + info.extension;
		const std::wstring assetMapFile = texturePath + info.name + L"_map." + info.extension;
		auto fullPath = ContentManager::GetFullAssetPath(assetDiffuseFile);
		OpacityMaterial* pOpMaterial = MaterialManager::Get()->CreateMaterial<OpacityMaterial>();
		if (fs::exists(fullPath))
		{
			pOpMaterial->SetDiffuseTexture(assetDiffuseFile);
		}

		fullPath = ContentManager::GetFullAssetPath(assetMapFile);
		if (fs::exists(fullPath))
		{
			pOpMaterial->SetOpacityTexture(assetMapFile);
			pOpMaterial->SetUseOpacityMap(true);
		}

		return pOpMaterial;
	}

	inline DiffuseMaterial* CreateDiffuseMaterial(const std::wstring& texturePath, const TextureInfo& info)
	{
		const std::wstring assetDiffuseFile = texturePath + info.name + L"_diffuse." + info.extension;
		const auto fullPath = ContentManager::GetFullAssetPath(assetDiffuseFile);
		DiffuseMaterial* pOpMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
		if (fs::exists(fullPath))
		{
			pOpMaterial->SetDiffuseTexture(assetDiffuseFile);
		}

		return pOpMaterial;
	}

	inline void AddTextureToModelComponent(ModelComponent* pModelComp, const std::wstring& dataFilePath) {

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
		json jsonData = *ContentManager::Load<json>(dataFilePath);

		std::string filePath = jsonData["texturePath"];
		std::wstring filePathW(filePath.length(), L' '); // Make room for characters

		// Copy string to wstring.
		std::copy(filePath.begin(), filePath.end(), filePathW.begin());

		json textures = jsonData["textures"];
		auto texturesSize = textures.size();

		auto meshes = pModelComp->GetMeshFilter()->GetMeshes();
		meshes[0].name = L"test";

		for (size_t i{}; i < texturesSize; ++i)
		{
			TextureInfo info{};
			info.meshIdx = textures.at(i)["meshIndex"];
			info.type = static_cast<MaterialType>(textures.at(i)["type"]);
			//...
			std::string strExtension = textures.at(i)["extension"];
			std::wstring strExtension2(strExtension.length(), L' '); // Make room for characters

			// Copy string to wstring.
			std::copy(strExtension.begin(), strExtension.end(), strExtension2.begin());
			info.extension = strExtension2;

			//...
			std::string strName = textures.at(i)["name"];
			std::wstring strName2(strName.length(), L' '); // Make room for characters

			// Copy string to wstring.
			std::copy(strName.begin(), strName.end(), strName2.begin());
			info.name = strName2;

			switch (info.type)
			{
			case MaterialType::opacity:
			{
				auto pMat = CreateOpacityMaterial(filePathW, info);
				UINT8 meshId = static_cast<UINT8>(info.meshIdx);
				pModelComp->SetMaterial(pMat, meshId);
				break;
			}
			case MaterialType::diffuse:
			{
				auto pMat = CreateDiffuseMaterial(filePathW, info);
				UINT8 meshId = static_cast<UINT8>(info.meshIdx);
				pModelComp->SetMaterial(pMat, meshId);
				break;
			}
			case MaterialType::skinned:
			{
				auto pMat = CreateDiffuseSkinnedMaterial(filePathW, info);
				UINT8 meshId = static_cast<UINT8>(info.meshIdx);
				pModelComp->SetMaterial(pMat, meshId);
				break;
			}
			case MaterialType::shadowSkinned:
			{
				auto pMat = CreateDiffuseShadowSkinnedMaterial(filePathW, info);
				UINT8 meshId = static_cast<UINT8>(info.meshIdx);
				pModelComp->SetMaterial(pMat, meshId);
				break;
			}
			case MaterialType::shadow:
			{
				auto pMat = CreateDiffuseShadowMaterial(filePathW, info);
				UINT8 meshId = static_cast<UINT8>(info.meshIdx);
				pModelComp->SetMaterial(pMat, meshId);
				break;
			}
			default:;
			}
		}
	}

	inline FMOD_VECTOR ToFmod(XMFLOAT3 v)
	{
		return FMOD_VECTOR(v.x, v.y, v.z);
	}
}
