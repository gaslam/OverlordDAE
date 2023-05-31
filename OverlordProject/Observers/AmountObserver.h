#pragma once
#include "Observer/Observer.h"

class Event;
class AmountObserver : public Observer
{
public:
	AmountObserver() = default;
	void OnNotify(GameObject* object, Event& event) override;
private:
	void SetText(GameObject* object);
};

enum EventType : int {
	NUMBER_CHANGED
};

