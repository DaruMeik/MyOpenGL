#include "Util/util.h"
#include "Util/shader.h"
#include "Util/vertex_buffer.h"
#include "Util/vertex_array.h"
#include "Util/index_buffer.h"

#include "Game/game_object.h"
#include "Script/event_system.h"
#include "Script/game_manager.h"

#pragma region game setting
const std::vector<int> POSSIBLE_FPS = { 50, 55, 60 };
int FRAMES_PER_SECOND = 2;
int SKIP_TICKS = 1000 / POSSIBLE_FPS[FRAMES_PER_SECOND];

float GRAVITY = 9.8f;
#pragma endregion

#pragma region callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
#pragma endregion

void readInput(GLFWwindow* window, std::vector<std::pair<bool, bool>>& input)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	switch (glfwGetKey(window, GLFW_KEY_SPACE))
	{
	case GLFW_PRESS:
		input[0].first = true;
		break;
	case GLFW_RELEASE:
		input[0].first = false;
		break;
	default:
		ASSERT(false);
		break;
	}

	switch (glfwGetKey(window, GLFW_KEY_R))
	{
	case GLFW_PRESS:
		input[1].first = true;
		break;
	case GLFW_RELEASE:
		input[1].first = false;
		break;
	default:
		ASSERT(false);
		break;
	}
}
void spawn(std::set<GameObject*>* gObjs, std::queue<GameObject*>* spawnedObjs)
{
	while (!spawnedObjs->empty())
	{
		auto gObj = spawnedObjs->front();
		spawnedObjs->pop();
		gObj->Awake();
		gObj->SetEnable(true);
		gObjs->insert(gObj);
		//std::cout << "About to spawn: " << gObj->name << std::endl;
	}
}
void clean(std::set<GameObject*>* gObjs, std::queue<GameObject*>* destroyedObjs)
{
	while (!destroyedObjs->empty())
	{
		auto gObj = destroyedObjs->front();
		destroyedObjs->pop();
		gObjs->erase(gObj);
		delete gObj;
	}
}

void update(GLFWwindow* window,
	std::vector<std::pair<bool, bool>>* input,
	std::set<GameObject*>* gObjs)
{
	readInput(window, *input);

	for (auto gObj : *gObjs)
	{
		//std::cout << gObj->name << std::endl;
		gObj->Update(*input);
	}
}

void render(GLFWwindow* window, Texture* textureList, std::set<GameObject*>* gObjs, Shader* shader, glm::mat4* projection, glm::mat4* view)
{
	glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (gObjs->size() <= 0)
	{
		glfwSwapBuffers(window);
		return;
	}

	shader->Bind();
	shader->SetUniformMat4f("projection", *projection);
	shader->SetUniformMat4f("view", *view);

	for (auto gObj : *gObjs)
	{
		if (gObj->shapeInfo.shape == Shape::S_EMPTY || !gObj->isEnabled)
			continue;

		//std::cout << "Render " << gObj->name << " with texture at slot " << gObj->textureSlot << "." << std::endl;
		textureList->Bind(gObj->textureSlot);
		shader->SetUniformMat4f("model", gObj->modelMatrix);
		shader->SetUniform1i("u_Texture", gObj->textureSlot);

		gObj->va->Bind();
		gObj->vb->Bind();
		gObj->ib->Bind();

		glDrawElements(GL_TRIANGLES, gObj->shapeInfo.indices.size(), GL_UNSIGNED_INT, 0);
	}

	glfwSwapBuffers(window);
}

void increaseFPS()
{
	if (FRAMES_PER_SECOND >= POSSIBLE_FPS.size() - 1)
		return;
	FRAMES_PER_SECOND++;
	SKIP_TICKS = 1000 / POSSIBLE_FPS[FRAMES_PER_SECOND];
}

void reduceFPS()
{
	if (FRAMES_PER_SECOND == 0)
		return;
	FRAMES_PER_SECOND--;
	SKIP_TICKS = 1000 / POSSIBLE_FPS[FRAMES_PER_SECOND];
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetWindowSizeCallback(window, framebuffer_size_callback);

#pragma region not abstract yet stuffs
	// Camera stuffs
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -10.0f, 10.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

#pragma endregion

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Texture texture;
	std::set<GameObject*> gObjs;
	std::queue<GameObject*> spawnedObjs;
	std::queue<GameObject*> destroyedObjs;

	EventSystem eventSystem = {};

	std::vector<std::pair<bool, bool>> input{ 2, std::pair<bool,bool>{false,true} }; // .first = has pressed, .second = has released

	GameManager* gameManager = new GameManager(Shape::S_EMPTY, eventSystem, texture, gObjs, spawnedObjs, destroyedObjs);
	spawnedObjs.push(gameManager);

#pragma region va->vb->ib->shader

	// Shader
	Shader shader("Resource/Shader/basic.shader");
	shader.Bind();

	// Unbind
	shader.Unbind();
#pragma endregion


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//std::cout << "FPS: " << POSSIBLE_FPS[FRAMES_PER_SECOND] << std::endl;
		/* Spawn / Despawn object here*/
		clean(&gObjs, &destroyedObjs);
		spawn(&gObjs, &spawnedObjs);

		/* Update here */
		//std::thread updateThread(update, window, &input, &gObjs);
		update(window, &input, &gObjs);

		DWORD start = GetTickCount();

		/* Render here */
		render(window, &texture, &gObjs, &shader, &projection, &view);

		DWORD stop = GetTickCount();

		//updateThread.join();

		/* Poll for and process events */
		glfwPollEvents();

		// Sleep to wait for tick
		int sleep_time = SKIP_TICKS - (stop - start);
		if (sleep_time >= 0)
		{
			Sleep(sleep_time);
			if (FRAMES_PER_SECOND < POSSIBLE_FPS.size() - 1 && sleep_time >= 1000 / POSSIBLE_FPS[FRAMES_PER_SECOND + 1])
			{
				std::cout << "Reduce FPS" << std::endl;
				increaseFPS();
			}
		}
		else
		{
			std::cout << "Increase FPS" << std::endl;
			reduceFPS();
		}
	}
	
	destroyedObjs.push(gameManager);
	clean(&gObjs, &destroyedObjs);

	glfwTerminate();
	return 0;
}