#pragma once
#include "shape.h"
#include "../Util/vertex_array.h"
#include "../Util/index_buffer.h"

class GameObject
{
public:
	GameObject(ShapeDict::Shape shape, std::vector<GameObject*>& gObjs, std::vector<GameObject*>& destroyedObjs);

	std::vector<GameObject*>& container;
	std::vector<GameObject*>& destroyedContainer;
	ShapeInfo shapeInfo;

	glm::mat4 modelMatrix;


	VertexArray va;
	VertexBuffer vb;
	VertexBufferLayout layout;
	IndexBuffer ib;

	virtual void Start() {};
	virtual void Update(std::vector<bool>& input);
	void DestroyObject();

	void SetPosition(glm::vec3 pos);

protected:
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;

private:
	void PhysicsUpdate()
	{
		modelMatrix = glm::translate(modelMatrix, m_Velocity);

		m_Velocity += m_Acceleration;
	};
};