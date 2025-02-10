#include "player.h"

void Player::OnEnable()
{
	GameObject::OnEnable();
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
	name = "Player";
	spritePath = "Resource/Texture/Player.png";
	SetTexture();
	SetCollider(-0.4f, -0.4f, 0.8f, 0.8f);
	m_Acceleration = glm::vec3(0.0f, -9.8f, 0.0f) * 0.001f;
	m_ReadyToJump = true;
}
void Player::OnDisable()
{
	GameObject::OnDisable();
}
void Player::Update(std::vector<bool>& input)
{
	if (!isEnabled)
		return;

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
void Player::OnCollisionEnter(GameObject* gObj)
{
	std::cout << "Collide with " << gObj->name << std::endl;
}