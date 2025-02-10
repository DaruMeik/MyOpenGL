#pragma once
#include "../Util/util.h"

class EventSystem
{
public:
	// Event List:
	std::vector<std::function<void()>*> OnGameOverEvents;
	void OnGameOverCallback();

	std::vector<std::function<void()>*> OnScoreIncreaseEvents;
	void OnScoreIncreaseCallback();
};