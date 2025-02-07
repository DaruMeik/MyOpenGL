#pragma once
#include "../Game/game_object.h"
class Obstacle : public GameObject
{
public:
	using GameObject::GameObject;

	void Start() override;
	void Update(std::vector<bool>& input) override;
};