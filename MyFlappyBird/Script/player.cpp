#include "player.h"
void Player::Awake()
{
	SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
	name = "Player" + std::to_string(container.size());
	spritePath = "Resource/Texture/Player.png";
	SetTexture();
	SetCollider(-0.4f, -0.4f, 0.8f, 0.8f);
	m_Acceleration = glm::vec3(0.0f, -9.8f, 0.0f) * 0.001f;
	m_ReadyToJump = true;
	m_IsDead = false;
}

void Player::Update(std::vector<std::pair<bool, bool>>& input)
{
	if (!isEnabled)
		return;

	GameObject::Update(input);

	if (m_IsDead)
		return;

	if (input[0].first)
	{
		if (input[0].second)
		{
			//std::cout << "JUMPING!!" << std::endl;
			m_Velocity = glm::vec3(0.0f, 15.0f, 0.0f) * 0.01f;
			input[0].second = false;
		}
	}
	else
	{
		input[0].second = true;
	}

	if (modelMatrix[3].y < -3.5f)
	{
		m_IsDead = true;
		eventSystem.OnGameOverCallback();
	}
}

void Player::OnCollisionEnter(GameObject* gObj)
{
	if (m_IsDead)
		return;
	std::cout << "Collide with " << gObj->name << std::endl;
	m_IsDead = true;
	eventSystem.OnGameOverCallback();
}