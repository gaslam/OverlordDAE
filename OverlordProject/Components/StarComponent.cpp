#include "stdafx.h"
#include "StarComponent.h"
#include "Prefabs/StarObject.h"
#include <algorithm>

#include "Misc/json.hpp"

using json = nlohmann::json;

void StarComponent::Initialize(const SceneContext&)
{
	json jsonData = *ContentManager::Load<json>(L"GameData/Stars.json");
	const float scale = jsonData["scale"];
	json positions = jsonData.at("positions");

	const size_t totalStars = positions.size();
	for (size_t i{}; i < totalStars; ++i)
	{
		nlohmann::json position = positions.at(i);
		const XMFLOAT3 pos{ position["x"], position["y"], position["z"] };
		auto star = m_pStars.emplace_back(new StarObject{ pos ,scale });
		m_pScene->AddChild(star);
	}
}

void StarComponent::Update(const SceneContext&)
{
	RemoveStars();
}

void StarComponent::RemoveStars()
{
	auto it = std::remove_if(m_pStars.begin(), m_pStars.end(), [this](StarObject* object)
		{
			if (object->IsMarkedAsDeleted())
			{
				m_pScene->RemoveChild(object);
				return true;
			}
			return false;
		});
	if (it == m_pStars.end())
	{
		return;
	}
	m_pStars.erase(it);
}
