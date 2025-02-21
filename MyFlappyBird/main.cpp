#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Util/util.h"
#include "Util/shader.h"
#include "Util/vertex_buffer.h"
#include "Util/vertex_array.h"
#include "Util/index_buffer.h"

#include "Game/game_object.h"
#include "Script/game_stat.h"
#include "Script/event_system.h"
#include "Script/game_manager.h"

#pragma region game setting
int TARGET_FPS = 60;
double SKIP_TICKS = 1.0f / TARGET_FPS;

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
	std::set<GameObject*>* gObjs,
	double deltaTime)
{
	readInput(window, *input);

	for (auto gObj : *gObjs)
	{
		//std::cout << gObj->name << std::endl;
		gObj->Update(*input, deltaTime);
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

	// Init ImGUI

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

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

	double updateStartTime = glfwGetTime();
	double renderStartTime = glfwGetTime();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame(); 
		
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Game UI");                          // Create a window called "Hello, world!" and append into it.


			// Game Tutorial
			ImGui::Text("-- Control --");
			ImGui::Text("Space: Jump");
			ImGui::Text("R: Reset when game over");
			ImGui::Text("Q: Quit");

			ImGui::Text("Curent score: %d", GameStat::SCORE/2);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// To ensure that update is called on a constant FPS (Kinda like Unity FixedUpdate)
		if (glfwGetTime() >= updateStartTime + SKIP_TICKS)
		{
			/* Spawn / Despawn object here */
			clean(&gObjs, &destroyedObjs);
			spawn(&gObjs, &spawnedObjs);

			/* Update here */
			update(window, &input, &gObjs, glfwGetTime() - updateStartTime);
			updateStartTime = glfwGetTime();
		}

		/* Render here */


		render(window, &texture, &gObjs, &shader, &projection, &view);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		renderStartTime = glfwGetTime();

		glfwSwapBuffers(window);
	}
	
	destroyedObjs.push(gameManager);
	clean(&gObjs, &destroyedObjs);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}