#include "stdafx.h"
#include "CoinComponent.h"
#include "Prefabs/CoinObject.h"
#include <algorithm>

#include "Components/NumberDisplayComponent.h"
#include "Misc/json.hpp"
#include "Observer/Event.h"
#include "Observers/AmountObserver.h"

using json = nlohmann::json;

void CoinComponent::Initialize(const SceneContext&)
{
	m_AmountObserver = std::make_unique<AmountObserverCoins>();
	GameObject* gameObject = GetGameObject();
	m_NumberDisplayObject = gameObject->AddChild(new GameObject{});
	auto pComp = m_NumberDisplayObject->AddComponent(new NumberDisplayComponent{ L"Textures/UI/Coin_UI.png" });
	XMFLOAT2 textPos{ 600.f,13.33f };
	XMFLOAT2 iconPos{ 573.33f,40.f };
	XMFLOAT2 iconScale{ 0.400f,0.400f };
	pComp->SetTextPosition(textPos);
	pComp->SetIconPosition(iconPos);
	pComp->SetIconScale(iconScale);
	Event eventType = Event(EventType::NUMBER_CHANGED);
	m_AmountObserver->OnNotify(gameObject, eventType);
	AddObjects();
}

void CoinComponent::Update(const SceneContext&)
{
	RemoveCoins();
	auto count{ std::ranges::count_if(m_pCoins, [](const CoinObject* pObject)
{
	return pObject->IsWaitingForDeletion();
}) };
	const int countInt{ static_cast<int>(count) };
	if (m_CoinsCollected < countInt)
	{
		GameObject* gameObject = GetGameObject();
		m_CoinsCollected = countInt;
		Event eventType = Event(EventType::NUMBER_CHANGED);
		m_AmountObserver->OnNotify(gameObject, eventType);
	}
}

void CoinComponent::RemoveCoins()
{
	GameObject* gameObject = GetGameObject();
	auto it = std::find_if(m_pCoins.begin(), m_pCoins.end(), [gameObject,this](CoinObject* object)
	{
		if(object->IsMarkedAsDeleted())
		{
			gameObject->RemoveChild(object);
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

void CoinComponent::OnGUI()
{
	auto pComp = m_NumberDisplayObject->GetComponent<NumberDisplayComponent>();
	if (ImGui::CollapsingHeader("Coins UI") && pComp)
	{
		pComp->DrawImGUI();
	}
}

void CoinComponent::InitObjects()
{
	GameObject* gameObject = GetGameObject();
	for (auto star : m_pCoins)
	{
		gameObject->RemoveChild(star);
	}
	m_pCoins.clear();
	m_CoinsCollected = 0;
	Event eventType = Event(EventType::NUMBER_CHANGED);
	m_AmountObserver->OnNotify(gameObject, eventType);
	AddObjects();
}

void CoinComponent::AddObjects()
{
	json jsonData = *ContentManager::Load<json>(L"GameData/Coins.json");
	const float scale = jsonData["scale"];
	json positions = jsonData.at("positions");
	GameObject* gameObject = GetGameObject();
	const size_t totalStars = positions.size();
	for (size_t i{}; i < totalStars; ++i)
	{
		nlohmann::json position = positions.at(i);
		const XMFLOAT3 pos{ position["x"], position["y"], position["z"] };
		auto coin = m_pCoins.emplace_back(new CoinObject{ pos ,scale });
		gameObject->AddChild(coin);
	}
}
