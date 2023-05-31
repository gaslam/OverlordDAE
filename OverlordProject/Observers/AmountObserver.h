#pragma once
#include "Observer/Observer.h"

class Event;
class AmountObserverStars : public Observer
{
public:
	AmountObserverStars() = default;
	void OnNotify(GameObject* object, Event& event) override;
private:
	void SetText(GameObject* object);
};

class AmountObserverCoins : public Observer
{
public:
	AmountObserverCoins() = default;
	void OnNotify(GameObject* object, Event& event) override;
private:
	void SetText(GameObject* object);
};

enum EventType : int {
	NUMBER_CHANGED
};

