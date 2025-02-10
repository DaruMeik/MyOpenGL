#include "event_system.h"

void EventSystem::OnGameOverCallback()
{
	for (auto& f : OnGameOverEvents)
	{
		(*f)();
	}
}
void EventSystem::OnScoreIncreaseCallback()
{
	for (auto& f : OnScoreIncreaseEvents)
	{
		(*f)();
	}
}