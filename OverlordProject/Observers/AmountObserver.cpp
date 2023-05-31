#include "stdafx.h"
#include "AmountObserver.h"

#include "Components/NumberDisplayComponent.h"
#include "Components/StarComponent.h"
#include "Observer/Event.h"

void AmountObserver::OnNotify(GameObject* object, Event& event)
{
	if(event.GetId() == EventType::NUMBER_CHANGED)
	{
		SetText(object);
	}
}

void AmountObserver::SetText(GameObject* object)
{
	NumberDisplayComponent* pDisplayComp = object->GetComponent<NumberDisplayComponent>(true);
	StarComponent* pStarComponent = object->GetComponent<StarComponent>();
	if(pDisplayComp && pStarComponent)
	{
		int starCount = pStarComponent->GetStarCount();
		pDisplayComp->SetNumber(starCount);
	}
}
