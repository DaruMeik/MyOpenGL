#include "game_object.h"

GameObject::GameObject(ShapeDict::Shape shape, std::vector<GameObject*>& gObjs, std::vector<GameObject*>& destroyedObjs) :
	container{ gObjs },
	destroyedContainer{ destroyedObjs },
	modelMatrix{ glm::mat4(1.0f) },
	m_Velocity{ glm::vec3(0.0f) },
	m_Acceleration{ glm::vec3(0.0f) },
	shapeInfo{ ShapeDict::GetShapeInfo(shape) },
	va{},
	vb{ shapeInfo.vertices.data(), sizeof(GLfloat) * shapeInfo.vertices.size(), GL_STATIC_DRAW },
	layout{},
	ib{ shapeInfo.indices.data(), shapeInfo.indices.size() }
{
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	ib.Unbind();
	vb.Unbind();
	va.Unbind();
}

void GameObject::DestroyObject()
{
	std::cout << "Destroying!" << std::endl;
	for (int i = 0; i < container.size(); i++)
	{
		if (container[i] == this)
		{
			destroyedContainer.push_back(container[i]);
		}
	}
}

void GameObject::Update(std::vector<bool>& input)
{
	PhysicsUpdate();
};

void GameObject::SetPosition(glm::vec3 pos)
{
	modelMatrix[3] = glm::vec4(pos, 1.0f);
}