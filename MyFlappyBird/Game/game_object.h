#pragma once
#include "shape.h"
#include "../Util/vertex_array.h"
#include "../Util/index_buffer.h"

struct CollisionBox
{
	float x;
	float y;
	float xSize;
	float ySize;
	CollisionBox():
		x{ 0 }, y{ 0 }, xSize{ 0 }, ySize{ 0 }{ }
	CollisionBox(float x, float y, float xSize, float ySize) : 
		x{ x }, y{ y }, xSize{ xSize }, ySize{ ySize }{ }
};

class GameObject
{
public:
	GameObject(Shape shape,
		std::vector<GameObject*>& gObjs,
		std::vector<GameObject*>& spawnedObjs,
		std::vector<GameObject*>& destroyedObjs);

	std::vector<GameObject*>& container;
	std::vector<GameObject*>& spawnedContainer;
	std::vector<GameObject*>& destroyedContainer;
	ShapeInfo shapeInfo;

	std::string name;
	bool isEnabled;
	glm::mat4 modelMatrix;

	VertexArray* va;
	VertexBuffer* vb;
	VertexBufferLayout* layout;
	IndexBuffer* ib;

	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnCollisionEnter(GameObject* gObj) {};
	virtual void Update(std::vector<bool>& input);
	void CleanObject();

	void SetPosition(glm::vec3 pos);

	void SetCollider(float x, float y, float xSize, float ySize);

	void CheckCollision();

protected:
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;
	CollisionBox m_Collider;

private:
	void PhysicsUpdate();
};