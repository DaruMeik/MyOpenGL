#pragma once
#include "../Util/util.h"
#include "obstacle.h"

class ObstacleSpawner : public GameObject
{
public:

	using GameObject::GameObject;
	~ObstacleSpawner();

	void OnEnable() override;
	void OnDisable() override;
	void Update(std::vector<bool>& input) override;

	void SpawnObstacle();
	void ReturnObstacle(std::pair<Obstacle*, Obstacle*> obstacle);

private:
	ULONGLONG m_NextTimeToSpawn = 0;
	std::queue<std::pair<Obstacle*, Obstacle*>> m_readyPool;
	std::queue<std::pair<Obstacle*, Obstacle*>> m_usedPool;
};