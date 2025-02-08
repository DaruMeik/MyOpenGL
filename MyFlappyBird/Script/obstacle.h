#pragma once
#include "../Game/game_object.h"

class Obstacle : public GameObject
{
public:
	using GameObject::GameObject;

	void OnEnable() override;
	void OnDisable() override;
	void Update(std::vector<bool>& input) override;
};