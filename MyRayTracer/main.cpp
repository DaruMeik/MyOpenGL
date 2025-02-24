#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Util/util.h"
#include "Util/shader.h"
#include "Util/vertex_buffer.h"
#include "Util/vertex_array.h"
#include "Util/index_buffer.h"
#include "Util/texture.h"

#pragma region callback

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
int CURRENT_SHADER_INDEX = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
#pragma endregion

void readInput(GLFWwindow* window, std::vector<std::pair<bool, bool>>& input)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void update(GLFWwindow* window,
	std::vector<std::pair<bool, bool>>* input)
{
	readInput(window, *input);
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
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
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
	projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 model = glm::mat4(1.0f);

#pragma endregion

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<std::pair<bool, bool>> input{ 2, std::pair<bool,bool>{false,true} }; // .first = has pressed, .second = has released

#pragma region va->vb->ib->shader
	std::vector<float>vertices = std::vector<float>{
	-1.0f, -1.0f, 0.0f, 0.0f,
	+1.0f, -1.0f, 1.0f, 0.0f,
	+1.0f, +1.0f, 1.0f, 1.0f,
	-1.0f, +1.0f, 0.0f, 1.0f
	};
	std::vector<unsigned int>indices = std::vector<unsigned int>{
		0, 1, 3,
		1, 2, 3
	};

	VertexArray va{};
	VertexBuffer vb{ vertices.data(), sizeof(GLfloat) * vertices.size(), GL_STATIC_DRAW };
	VertexBufferLayout layout{};
	IndexBuffer ib{ indices.data(), indices.size() };

	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	// Shader
	Shader noiseSphereShader{ "Resource/Shader/noise_sphere.shader" };
	noiseSphereShader.Bind();
	noiseSphereShader.SetUniformMat4f("projection", projection);
	noiseSphereShader.SetUniformMat4f("view", view);
	noiseSphereShader.SetUniformMat4f("model", model);
	noiseSphereShader.SetUniform3f("iResolution", WIDTH, HEIGHT, 1.0f);
	noiseSphereShader.Unbind();
	
	Shader noiseSphereShaderOrigin{ "Resource/Shader/noise_sphere_2.shader" };
	noiseSphereShaderOrigin.Bind();
	noiseSphereShaderOrigin.SetUniformMat4f("projection", projection);
	noiseSphereShaderOrigin.SetUniformMat4f("view", view);
	noiseSphereShaderOrigin.SetUniformMat4f("model", model);
	noiseSphereShaderOrigin.SetUniform3f("iResolution", WIDTH, HEIGHT, 1.0f);
	noiseSphereShaderOrigin.Unbind();

	//// Texture
	//Texture texture{};
	//unsigned int textureSlot = texture.AddTexture("Resource/Texture/Player.png");

	// Unbind
	ib.Unbind();
	vb.Unbind();
	va.Unbind();
#pragma endregion

	/* Loop until the user closes the window */
	double startTime = glfwGetTime();
	double pauseTime = startTime;
	bool isPausing = false;
	while (!glfwWindowShouldClose(window))
	{
		/* Poll for and process events */
		glfwPollEvents();


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame(); 
		
		{
			ImGui::Begin("Control");                          // Create a window called "Hello, world!" and append into it.

			const char* shaders[] = { "Raycast Sphere" , "Noisy Sphere"};
			ImGui::Combo("Fragment Shader", &CURRENT_SHADER_INDEX, shaders, IM_ARRAYSIZE(shaders));

			if (ImGui::Button("Reset"))
			{
				startTime = glfwGetTime();
			}

			if (ImGui::Button("Pause"))
			{
				isPausing = !isPausing;
				if (isPausing)
					pauseTime = glfwGetTime();
				else
					startTime += glfwGetTime() - pauseTime;

			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		/* Update here */
		update(window, &input);
		

		/* Render here */
		glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Set up shader
		Shader* shader;
		switch (CURRENT_SHADER_INDEX)
		{
		case 0:
		default:
			shader = &noiseSphereShader;
			break;
		case 1:
			shader = &noiseSphereShaderOrigin;
			break;
		}

		shader->Bind();
		//texture.Bind(textureSlot);
		if(isPausing)
			shader->SetUniform1f("iTime", pauseTime - startTime);
		else
			shader->SetUniform1f("iTime", glfwGetTime() - startTime);
		//shader.SetUniform1i("u_Texture", textureSlot);


		va.Bind();
		vb.Bind();
		ib.Bind();

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}