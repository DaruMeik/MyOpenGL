#include "game_object.h"

GameObject::GameObject(Shape shape, Texture& texture, std::vector<GameObject*>& gObjs, std::vector<GameObject*>& spawnedObjs, std::vector<GameObject*>& destroyedObjs) :
	textureList{texture},
	container{ gObjs },
	spawnedContainer {spawnedObjs},
	destroyedContainer{ destroyedObjs },
	isEnabled{ false },
	modelMatrix{ glm::mat4(1.0f) },
	textureSlot{0},
	m_Velocity{ glm::vec3(0.0f) },
	m_Acceleration{ glm::vec3(0.0f) },
	shapeInfo{ ShapeDict::GetShapeInfo(shape) }
{
	if (shape != Shape::S_EMPTY)
	{
		va = new VertexArray{};
		vb = new VertexBuffer{ shapeInfo.vertices.data(), sizeof(GLfloat) * shapeInfo.vertices.size(), GL_STATIC_DRAW };
		layout = new VertexBufferLayout{};
		ib = new IndexBuffer{ shapeInfo.indices.data(), shapeInfo.indices.size() };
		SetTexture();

		layout->Push<float>(2);
		layout->Push<float>(2);
		va->AddBuffer(*vb, *layout);

		ib->Unbind();
		vb->Unbind();
		va->Unbind();
	}
}

void GameObject::OnEnable()
{
	isEnabled = true;
}

void GameObject::OnDisable()
{
	isEnabled = false;
}

void GameObject::CleanObject()
{
	std::cout << "Delete object" << std::endl;
	ib->Unbind();
	vb->Unbind();
	va->Unbind();
	delete ib;
	delete vb;
	delete layout;
	delete va;
	destroyedContainer.push_back(this);
}

void GameObject::Update(std::vector<bool>& input)
{
	if (!isEnabled)
		return;
	PhysicsUpdate();
	CheckCollision();
};

void GameObject::PhysicsUpdate()
{
	modelMatrix = glm::translate(modelMatrix, m_Velocity);

	m_Velocity += m_Acceleration;
}

void GameObject::SetTexture()
{
	textureSlot = textureList.AddTexture(spritePath);
}

void GameObject::SetPosition(glm::vec3 pos)
{
	modelMatrix[3] = glm::vec4(pos, 1.0f);
}

void GameObject::SetCollider(float x, float y, float xSize, float ySize)
{
	m_Collider.x = x;
	m_Collider.y = y;
	m_Collider.xSize = xSize;
	m_Collider.ySize = ySize;
}

void GameObject::CheckCollision()
{
	glm::vec4 lowerBound = modelMatrix * glm::vec4(m_Collider.x, m_Collider.y, 0.0f, 1.0f);
	glm::vec4 upperBound = modelMatrix * glm::vec4(m_Collider.x + m_Collider.xSize, m_Collider.y + m_Collider.ySize, 0.0f, 1.0f);

	for (auto gObj : container)
	{
		if (gObj == this || gObj->m_Collider.xSize == 0 || gObj->m_Collider.ySize == 0)
			continue;
		glm::vec3 t_lowerBound = gObj->modelMatrix * glm::vec4(gObj->m_Collider.x, gObj->m_Collider.y, 0.0f, 1.0f);
		glm::vec3 t_upperBound = gObj->modelMatrix * glm::vec4(gObj->m_Collider.x + gObj->m_Collider.xSize, gObj->m_Collider.y + gObj->m_Collider.ySize, 0.0f, 1.0f);

		// collision x-axis?
		bool collisionX = upperBound.x >= t_lowerBound.x &&
			t_upperBound.x >= lowerBound.x;

		// collision y-axis?
		bool collisionY = upperBound.y >= t_lowerBound.y &&
			t_upperBound.y >= lowerBound.y;
		if (collisionX && collisionY)
		{
			OnCollisionEnter(gObj);
			return;
		}
	}
}