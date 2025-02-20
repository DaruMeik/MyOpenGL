#pragma once
#include "../Util/util.h"
#include "obstacle_spawner.h"
#include "player.h"

class GameManager : public GameObject
{
public:

	using GameObject::GameObject;
	~GameManager();

	void Awake() override;
	void Update(std::vector<std::pair<bool, bool>>& input, double deltaTime) override;

private:
	unsigned int m_Score;
	Player* m_Player;
	ObstacleSpawner* m_ObstacleSpawner;

	void InitGame();

	std::function<void()> f_PrintScore = [&] {PrintScore(); };
	void PrintScore();

	std::function<void()> f_IncreaseScore = [&] {IncreaseScore(); };
	void IncreaseScore();
};
