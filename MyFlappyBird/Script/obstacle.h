#pragma once
#include "../Game/game_object.h"

class Obstacle : public GameObject
{
public:
	using GameObject::GameObject;
	~Obstacle();

	void Awake() override;
	void OnEnable() override;
	void Update(std::vector<std::pair<bool, bool>>& input) override;

private:
	bool hasScore = true;
	std::function<void()> f_StopMovement = [&] {StopMovement(); };
	void StopMovement();
};