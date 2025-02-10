#pragma once
#include "shape.h"
#include "../Util/vertex_array.h"
#include "../Util/index_buffer.h"
#include "../Util/texture.h"
#include "../Script/event_system.h"

struct CollisionBox
{
	float x;
	float y;
	float xSize;
	float ySize;
	CollisionBox() :
		x{ 0 }, y{ 0 }, xSize{ 0 }, ySize{ 0 }
	{
	}
	CollisionBox(float x, float y, float xSize, float ySize) :
		x{ x }, y{ y }, xSize{ xSize }, ySize{ ySize }
	{
	}
};

class GameObject
{
public:
	GameObject(Shape shape,
		EventSystem& eventSystem,
		Texture& textureList,
		std::set<GameObject*>& gObjs,
		std::queue<GameObject*>& spawnedObjs,
		std::queue<GameObject*>& destroyedObjs);
	virtual ~GameObject();

	Texture& textureList;
	EventSystem& eventSystem;
	std::set<GameObject*>& container;
	std::queue<GameObject*>& spawnedContainer;
	std::queue<GameObject*>& destroyedContainer;
	ShapeInfo shapeInfo;

	std::string name = "Null";
	std::string spritePath = "Resource/Texture/wall.jpg";
	bool isEnabled;
	glm::mat4 modelMatrix;

	VertexArray* va = nullptr;
	VertexBuffer* vb = nullptr;
	VertexBufferLayout* layout = nullptr;
	IndexBuffer* ib = nullptr;
	unsigned int textureSlot = 0;

	virtual void Awake() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void OnCollisionEnter(GameObject* gObj) {};
	virtual void Update(std::vector<std::pair<bool, bool>>& input);

	void SetEnable(bool enabled);

	void CleanObject();

	void SetTexture();

	void SetVelocity(glm::vec3 vel);

	void SetAccelaration(glm::vec3 accel);

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