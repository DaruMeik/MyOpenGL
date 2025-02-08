#include "obstacle.h"

void Obstacle::OnEnable()
{
	GameObject::OnEnable();
	SetCollider(-0.5f, -0.5f, 1.0f, 1.0f);
	name = "Obstacle";
	m_Velocity = glm::vec3(-10.0f, 0.0f, 0.0f) * 0.01f;
}

void Obstacle::OnDisable()
{
	GameObject::OnDisable();
}

void Obstacle::Update(std::vector<bool>& input)
{
	if (!isEnabled)
		return;

	GameObject::Update(input);
	
	if (modelMatrix[3].x < -6)
	{
		OnDisable();
	}
}