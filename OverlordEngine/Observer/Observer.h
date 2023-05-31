#pragma once
#include <memory>

class Event;
class GameObject;
class Observer
{
public:
	explicit Observer() = default;
	virtual ~Observer() = default;

	Observer(const Observer& other) = delete;
	Observer(Observer&& other) = delete;
	Observer& operator=(const Observer& other) = delete;
	Observer& operator=(Observer&& other) = delete;
	virtual void OnNotify(GameObject* object, Event& event) = 0;
};

