#include "stdafx.h"
#include "StarComponent.h"
#include "Prefabs/StarObject.h"
#include <algorithm>

#include "Components/NumberDisplayComponent.h"
#include "Misc/json.hpp"
#include "Observer/Event.h"

using json = nlohmann::json;

void StarComponent::Initialize(const SceneContext&)
{
	json jsonData = *ContentManager::Load<json>(L"GameData/Stars.json");
	const float scale = jsonData["scale"];
	json positions = jsonData.at("positions");

	const size_t totalStars = positions.size();
	GameObject* gameObject = GetGameObject();
	for (size_t i{}; i < totalStars; ++i)
	{
		nlohmann::json position = positions.at(i);
		const XMFLOAT3 pos{ position["x"], position["y"], position["z"] };
		auto star = m_pStars.emplace_back(new StarObject{ pos ,scale });
		gameObject->AddChild(star);
	}
	m_NumberDisplayObject = gameObject->AddChild(new GameObject{});
	auto pComp = m_NumberDisplayObject->AddComponent(new NumberDisplayComponent{ L"Textures/UI/Star_UI.png" });
	XMFLOAT2 textPos{ 1200.f,13.33f };
	XMFLOAT2 iconPos{ 1140.f,50.f };
	XMFLOAT2 iconScale{ 0.300f,0.300f };
	pComp->SetTextPosition(textPos);
	pComp->SetIconPosition(iconPos);
	pComp->SetIconScale(iconScale);
	m_AmountObserver = std::make_unique<AmountObserver>();
	Event eventType = Event(EventType::NUMBER_CHANGED);
	m_AmountObserver->OnNotify(gameObject, eventType);
}

void StarComponent::Update(const SceneContext&)
{
	RemoveStars();
}

void StarComponent::RemoveStars()
{
	GameObject* gameObject = GetGameObject();
	int starsToAdd{};
	auto it = std::remove_if(m_pStars.begin(), m_pStars.end(), [gameObject,&starsToAdd,this](StarObject* object)
		{
			if (object->IsMarkedAsDeleted())
			{
				if (object->IsFinalStar())
				{
					m_pScene->End();
				}
				gameObject->RemoveChild(object);
				++starsToAdd;
				return true;
			}
			return false;
		});
	if (it == m_pStars.end())
	{
		return;
	}
	m_pStars.erase(it);
	m_StarsCollected += starsToAdd;
	Event eventType = Event(EventType::NUMBER_CHANGED);
	m_AmountObserver->OnNotify(gameObject, eventType);
}

void StarComponent::OnGUI()
{
	GameObject* object{ GetGameObject() };
	auto pComp = object->GetComponent<NumberDisplayComponent>();
	if(ImGui::CollapsingHeader("Stars UI") && pComp)
	{
		pComp->DrawImGUI();
	}
}
