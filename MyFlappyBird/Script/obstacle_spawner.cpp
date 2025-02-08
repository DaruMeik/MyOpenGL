#include "obstacle_spawner.h"
#include <random>

std::random_device rd;
std::mt19937 gen;
std::uniform_real_distribution<> dis(-2.0f, 2.0f);

ObstacleSpawner::~ObstacleSpawner()
{
	GameObject::~GameObject();
	//while (!spawnedObjs.empty())
	//{
	//    delete spawnedObjs.top();
	//    spawnedObjs.pop();
	//}
	//Obstacle::spawner = this;
}

void ObstacleSpawner::OnEnable()
{
	GameObject::OnEnable();
	m_NextTimeToSpawn = GetTickCount64() + 2 * 1000;
	gen.seed(rd());
}
void ObstacleSpawner::OnDisable()
{
	GameObject::OnDisable();
}
void ObstacleSpawner::Update(std::vector<bool>& input)
{
	if (!isEnabled)
		return;
	GameObject::Update(input);

	while (!m_usedPool.empty() && !m_usedPool.front().first->isEnabled)
	{
		m_readyPool.push(m_usedPool.front());
		m_usedPool.pop();
	}

	if (m_NextTimeToSpawn <= GetTickCount64())
	{
		m_NextTimeToSpawn = GetTickCount64() + 2 * 1000;
		SpawnObstacle();
	}
}
void ObstacleSpawner::SpawnObstacle()
{
	std::pair<Obstacle*, Obstacle*> obj;
	if (m_readyPool.empty())
	{
		obj.first = new Obstacle(Shape::S_RECT, container, spawnedContainer, destroyedContainer);
		obj.first->modelMatrix = glm::scale(obj.first->modelMatrix, glm::vec3(1.0f, 4.0f, 1.0f));
		spawnedContainer.push_back(obj.first);
		obj.second = new Obstacle(Shape::S_RECT, container, spawnedContainer, destroyedContainer);
		obj.second->modelMatrix = glm::scale(obj.second->modelMatrix, glm::vec3(1.0f, 4.0f, 1.0f));
		spawnedContainer.push_back(obj.second);
	}
	else
	{
		obj = m_readyPool.front();
		m_readyPool.pop();
		obj.first->OnEnable();
		obj.second->OnEnable();
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