#include "obstacle_spawner.h"
#include "game_stat.h"
#include <random>

std::random_device rd;
std::mt19937 gen;
std::uniform_real_distribution<> dis(-2.0f, 2.0f);

ObstacleSpawner::~ObstacleSpawner()
{
	while (!m_readyPool.empty())
	{
		auto p = m_readyPool.front(); m_readyPool.pop();
		destroyedContainer.push(p.first);
		destroyedContainer.push(p.second);
	}
	while (!m_usedPool.empty())
	{
		auto p = m_usedPool.front(); m_usedPool.pop();
		destroyedContainer.push(p.first);
		destroyedContainer.push(p.second);
	}

	auto it = std::find(eventSystem.OnGameOverEvents.begin(), eventSystem.OnGameOverEvents.end(), &f_StopSpawning);
	eventSystem.OnGameOverEvents.erase(it);
}

void ObstacleSpawner::Awake()
{
	m_NextTimeToSpawn = glfwGetTime() + GameStat::OBSTACLE_SPAWN_TIME;
	name = "Obstacle Spawner" + std::to_string(container.size());
	gen.seed(rd());
	m_IsDead = false;
	eventSystem.OnGameOverEvents.push_back(&f_StopSpawning);
}

void ObstacleSpawner::Update(std::vector<std::pair<bool, bool>>& input, double deltaTime)
{
	if (!isEnabled)
		return;
	GameObject::Update(input, deltaTime);

	while (!m_usedPool.empty() && !m_usedPool.front().first->isEnabled)
	{
		m_readyPool.push(m_usedPool.front());
		m_usedPool.pop();
	}

	if (!m_IsDead && m_NextTimeToSpawn <= glfwGetTime())
	{
		m_NextTimeToSpawn = glfwGetTime() + GameStat::OBSTACLE_SPAWN_TIME;
		SpawnObstacle();
	}
}
void ObstacleSpawner::SpawnObstacle()
{
	std::pair<Obstacle*, Obstacle*> obj;
	if (m_readyPool.empty())
	{
		obj.first = new Obstacle(Shape::S_RECT, eventSystem, textureList, container, spawnedContainer, destroyedContainer);
		obj.first->modelMatrix = glm::scale(obj.first->modelMatrix, glm::vec3(1.0f, 4.0f, 1.0f));
		spawnedContainer.push(obj.first);
		obj.second = new Obstacle(Shape::S_RECT, eventSystem, textureList, container, spawnedContainer, destroyedContainer);
		obj.second->modelMatrix = glm::scale(obj.second->modelMatrix, glm::vec3(1.0f, 4.0f, 1.0f));
		spawnedContainer.push(obj.second);
	}
	else
	{
		obj = m_readyPool.front();
		m_readyPool.pop();
		obj.first->SetEnable(true);
		obj.second->SetEnable(true);
	}
	float height = dis(gen);
	obj.first->SetPosition(glm::vec3(6.0f, height + 3.0f, 0.0f));
	obj.second->SetPosition(glm::vec3(6.0f, height - 3.0f, 0.0f));
	m_usedPool.push(obj);
}
void ObstacleSpawner::ReturnObstacle(std::pair<Obstacle*, Obstacle*> obstacle)
{
	m_readyPool.push(obstacle);
}

void ObstacleSpawner::StopSpawning()
{
	m_IsDead = true;
}