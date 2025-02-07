#include "player.h"

void Player::Start()
{
	//m_Acceleration = glm::vec3(0.0f, -9.8f, 0.0f) * 0.001f;
	m_ReadyToJump = true;
}
void Player::Update(std::vector<bool>& input)
{
	GameObject::Update(input);

	if (input[0])
	{
		if (m_ReadyToJump)
		{
			m_Velocity = glm::vec3(0.0f, 15.0f, 0.0f) * 0.01f;
			m_ReadyToJump = false;
		}
	}
	else
	{
		m_ReadyToJump = true;
	}
}