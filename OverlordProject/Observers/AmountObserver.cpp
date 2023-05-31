#include "stdafx.h"
#include "AmountObserver.h"

#include "Components/CoinComponent.h"
#include "Components/NumberDisplayComponent.h"
#include "Components/StarComponent.h"
#include "Observer/Event.h"

void AmountObserverStars::OnNotify(GameObject* object, Event& event)
{
	if(event.GetId() == EventType::NUMBER_CHANGED)
	{
		SetText(object);
	}
}

void AmountObserverStars::SetText(GameObject* object)
{
	NumberDisplayComponent* pDisplayComp = object->GetComponent<NumberDisplayComponent>(true);
	StarComponent* pStarComponent = object->GetComponent<StarComponent>();
	if(pDisplayComp && pStarComponent)
	{
		int starCount = pStarComponent->GetStarCount();
		pDisplayComp->SetNumber(starCount);
	}
}

void AmountObserverCoins::OnNotify(GameObject* object, Event& event)
{
	if (event.GetId() == EventType::NUMBER_CHANGED)
	{
		SetText(object);
	}
}

void AmountObserverCoins::SetText(GameObject* object)
{
	NumberDisplayComponent* pDisplayComp = object->GetComponent<NumberDisplayComponent>(true);
	CoinComponent* pCoinComponent = object->GetComponent<CoinComponent>();
	if (pDisplayComp && pCoinComponent)
	{
		int coinCount = pCoinComponent->GetCoinsCount();
		pDisplayComp->SetNumber(coinCount);
	}
}
