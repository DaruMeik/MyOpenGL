#include "obstacle.h"

void Obstacle::Start()
{
	m_Velocity = glm::vec3(0.0f, -15.0f, 0.0f) * 0.01f;
}

void Obstacle::Update(std::vector<bool>& input)
{
	GameObject::Update(input);
}