#include "obstacle.h"
#include "game_stat.h"

Obstacle::~Obstacle()
{
	auto it = std::find(eventSystem.OnGameOverEvents.begin(), eventSystem.OnGameOverEvents.end(), &f_StopMovement);
	eventSystem.OnGameOverEvents.erase(it);
}

void Obstacle::Awake()
{
	SetCollider(-0.5f, -0.5f, 1.0f, 1.0f);
	name = "Obstacle" + std::to_string(container.size());
	m_Velocity = glm::vec3(-1.0f, 0.0f, 0.0f) * GameStat::OBSTACLE_SPEED;
	eventSystem.OnGameOverEvents.push_back(&f_StopMovement);
}

void Obstacle::OnEnable()
{
	hasScore = true;
}

void Obstacle::Update(std::vector<std::pair<bool, bool>>& input, double deltaTime)
{
	if (!isEnabled)
		return;

	GameObject::Update(input, deltaTime);
	
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