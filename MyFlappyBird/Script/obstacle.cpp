#include "obstacle.h"

Obstacle::~Obstacle()
{
	auto it = std::find(eventSystem.OnGameOverEvents.begin(), eventSystem.OnGameOverEvents.end(), &f_StopMovement);
	eventSystem.OnGameOverEvents.erase(it);
}

void Obstacle::Awake()
{
	SetCollider(-0.5f, -0.5f, 1.0f, 1.0f);
	name = "Obstacle" + std::to_string(container.size());
	m_Velocity = glm::vec3(-10.0f, 0.0f, 0.0f) * 0.01f;
	eventSystem.OnGameOverEvents.push_back(&f_StopMovement);
}

void Obstacle::OnEnable()
{
	hasScore = true;
}

void Obstacle::Update(std::vector<std::pair<bool, bool>>& input)
{
	if (!isEnabled)
		return;

	GameObject::Update(input);
	
	if (modelMatrix[3].x <= -6)
	{
		SetEnable(false);
	}
	else if (hasScore && modelMatrix[3].x <= -4)
	{
		hasScore = false;
		eventSystem.OnScoreIncreaseCallback();
	}
}

void Obstacle::StopMovement()
{
	m_Velocity = glm::vec3(0.0f);
}