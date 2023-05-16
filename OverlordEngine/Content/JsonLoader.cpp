#include "stdafx.h"
#include "JsonLoader.h"

json* JsonLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	std::ifstream fileStream{ loadInfo.assetFullPath };

	if(!fileStream.is_open())
	{
		Logger::LogWarning(L"Failed to open the file!\n\nFilepath: {}", loadInfo.assetFullPath.wstring());
		return nullptr;
	}

	json* data{ new json{ json::parse(fileStream)} };

	return data;
}

void JsonLoader::Destroy(json* objToDestroy)
{
	delete objToDestroy;
}
