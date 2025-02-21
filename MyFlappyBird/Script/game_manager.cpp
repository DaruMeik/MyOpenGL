#include "game_manager.h"
#include "game_stat.h"

GameManager::~GameManager()
{
	destroyedContainer.push(m_Player);
	destroyedContainer.push(m_ObstacleSpawner);

	auto it = std::find(eventSystem.OnGameOverEvents.begin(), eventSystem.OnGameOverEvents.end(), &f_PrintScore);
	eventSystem.OnGameOverEvents.erase(it);
}

void GameManager::Awake()
{
	name = "Game Manager";
	InitGame();
	eventSystem.OnScoreIncreaseEvents.push_back(&f_IncreaseScore);
	eventSystem.OnGameOverEvents.push_back(&f_PrintScore);
}

void GameManager::Update(std::vector<std::pair<bool, bool>>& input, double deltaTime)
{
	if (input[1].first)
	{
		if (input[1].second)
		{
			InitGame();
			input[1].second = false;
		}
	}
	else
	{
		input[1].second = true;
	}
}

void GameManager::InitGame()
{
	if (m_Player != nullptr)
	{
		destroyedContainer.push(m_Player);
	}
	if (m_ObstacleSpawner != nullptr)
	{
		destroyedContainer.push(m_ObstacleSpawner);
	}
	
	m_Player = new Player(Shape::S_RECT, eventSystem, textureList, 
		container, spawnedContainer, destroyedContainer);
	spawnedContainer.push(m_Player);

	m_ObstacleSpawner = new ObstacleSpawner(Shape::S_EMPTY, eventSystem, textureList, 
		container, spawnedContainer, destroyedContainer);
	spawnedContainer.push(m_ObstacleSpawner);

	GameStat::SCORE = 0;
}

void GameManager::PrintScore()
{
	std::cout << "Final score: " << GameStat::SCORE/2 << std::endl;
	std::cout << "Press R to reset the game. （Rを押してゲームをリセットしてください）" << std::endl;
}

void GameManager::IncreaseScore()
{
	GameStat::SCORE++;
}