#include "stdafx.h"
#include "CoinComponent.h"
#include "Prefabs/CoinObject.h"
#include <algorithm>

#include "Misc/json.hpp"

using json = nlohmann::json;

void CoinComponent::Initialize(const SceneContext&)
{
	json jsonData = *ContentManager::Load<json>(L"GameData/Coins.json");
	const float scale = jsonData["scale"];
	json positions = jsonData.at("positions");

	const size_t totalStars = positions.size();
	for (size_t i{}; i < totalStars; ++i)
	{
		nlohmann::json position = positions.at(i);
		const XMFLOAT3 pos{ position["x"], position["y"], position["z"] };
		auto coin = m_pCoins.emplace_back(new CoinObject{ pos ,scale });
		m_pScene->AddChild(coin);
	}
}

void CoinComponent::Update(const SceneContext&)
{
	RemoveCoins();
}

void CoinComponent::RemoveCoins()
{
	auto it = std::remove_if(m_pCoins.begin(), m_pCoins.end(), [this](CoinObject* object)
	{
		if(object->IsMarkedAsDeleted())
		{
			m_pScene->RemoveChild(object);
			return true;
		}
		return false;
	});
	if (it == m_pCoins.end())
	{
		return;
	}
	m_pCoins.erase(it);
}