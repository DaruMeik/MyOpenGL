#pragma once
#include "../Game/game_object.h"

class Player : public GameObject
{
public:
	using GameObject::GameObject;

	void OnEnable() override;
	void OnDisable() override;
	void Update(std::vector<bool>& input) override;
	void OnCollisionEnter(GameObject* gObj) override;
private:
	bool m_ReadyToJump = true;
};