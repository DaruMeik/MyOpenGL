#pragma once
#include "../Game/game_object.h"

class Player : public GameObject
{
public:
	using GameObject::GameObject;

	void Awake() override;
	void Update(std::vector<std::pair<bool, bool>>& input) override;
	void OnCollisionEnter(GameObject* gObj) override;
private:
	bool m_ReadyToJump = true;
	bool m_IsDead = false;
};