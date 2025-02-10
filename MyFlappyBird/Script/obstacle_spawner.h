#pragma once
#include "../Util/util.h"
#include "obstacle.h"

class ObstacleSpawner : public GameObject
{
public:

	using GameObject::GameObject;
	~ObstacleSpawner();

	void Awake() override;
	void Update(std::vector<std::pair<bool, bool>>& input) override;

	void SpawnObstacle();
	void ReturnObstacle(std::pair<Obstacle*, Obstacle*> obstacle);

private:
	ULONGLONG m_NextTimeToSpawn = 0;
	bool m_IsDead = false;
	std::queue<std::pair<Obstacle*, Obstacle*>> m_readyPool;
	std::queue<std::pair<Obstacle*, Obstacle*>> m_usedPool;

	std::function<void()> f_StopSpawning = [&] {StopSpawning(); };
	void StopSpawning();
};