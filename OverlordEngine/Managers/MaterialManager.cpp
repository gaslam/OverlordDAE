#include "stdafx.h"
#include "MaterialManager.h"

MaterialManager::~MaterialManager()
{

	m_Materials.clear();

	m_MaterialsPP.clear();
}

BaseMaterial* MaterialManager::GetMaterial(UINT materialId) const
{
	BaseMaterial* pBase{};
	if (IsBaseMaterial(materialId))
	{
		if (materialId < m_Materials.size())
		{
			pBase = m_Materials[materialId].get();
			return pBase;
		}
	}

	if (pBase == nullptr)
	{
		Logger::LogWarning(L"Material with ID={} not found", materialId);
	}

	return nullptr;
}

PostProcessingMaterial* MaterialManager::GetMaterial_Post(UINT materialId) const
{
	PostProcessingMaterial* pBase{};
	if (IsPostProcessingMaterial(materialId))
	{
		materialId = FromPPID(materialId);
		if (materialId < m_MaterialsPP.size())
		{
			pBase = m_MaterialsPP[materialId].get();
			return pBase;
		}
	}

	if (pBase == nullptr)
	{
		Logger::LogWarning(L"Material with ID={} not found", materialId);
	}

	return nullptr;
}

void MaterialManager::RemoveMaterial(UINT materialId, bool deleteObj)
{
	//Valid Material id?
	if (!IsValid(materialId))
	{
		Logger::LogWarning(L"MaterialManager::RemoveMaterial >> Invalid Material ID");
		return;
	}

	if(IsBaseMaterial(materialId))
	{
		if (m_Materials.size() < materialId || !m_Materials[materialId])
		{
			Logger::LogWarning(L"MaterialManager::RemoveMaterial >> Invalid Material ID");
			return;
		}

		m_Materials[materialId]->ResetMaterialId();

		if (deleteObj)
		{
			m_Materials[materialId].reset();
		}
		m_Materials[materialId] = nullptr;
	}
	else
	{
		materialId = FromPPID(materialId);
		if (m_MaterialsPP.size() < materialId || !m_MaterialsPP[materialId])
		{
			Logger::LogWarning(L"MaterialManager::RemoveMaterial >> Invalid Material ID");
			return;
		}

		m_MaterialsPP[materialId]->ResetMaterialId();

		if (deleteObj)
		{
			m_MaterialsPP[materialId].reset();
		}

		m_MaterialsPP[materialId] = nullptr;
	}
	
}

void MaterialManager::RemoveMaterial(BaseMaterial* pMaterial, bool deleteObj)
{
	const auto materialId = pMaterial->GetMaterialId();
	if (!IsValid(materialId) || m_Materials.size() < materialId || !m_Materials[materialId])
	{
		Logger::LogWarning(L"MaterialManager::RemoveMaterial >> Invalid Material ID");
		return;
	}

	if(m_Materials[materialId].get() == pMaterial)
	{
		RemoveMaterial(materialId, deleteObj);
	}
}

void MaterialManager::RemoveMaterial(PostProcessingMaterial* pMaterial, bool deleteObj)
{
	const auto materialId = pMaterial->GetMaterialId();
	if (!IsValid(materialId) || m_MaterialsPP.size() < materialId || !m_MaterialsPP[materialId])
	{
		Logger::LogWarning(L"MaterialManager::RemoveMaterial >> Invalid Material ID");
		return;
	}

	if (m_MaterialsPP[materialId].get() == pMaterial)
	{
		RemoveMaterial(materialId, deleteObj);
	}
}
