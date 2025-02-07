#include "Util/util.h"
#include "Util/shader.h"
#include "Util/vertex_buffer.h"
#include "Util/vertex_array.h"
#include "Util/index_buffer.h"

#include "Game/game_object.h"
#include "Script/player.h"
#include "Script/obstacle.h"

#pragma region game setting
const int POSSIBLE_FPS[] = {30, 60, 120, 144};
int FRAMES_PER_SECOND = 0;
int SKIP_TICKS = 1000 / POSSIBLE_FPS[FRAMES_PER_SECOND];

float GRAVITY = 9.8f;
#pragma endregion

#pragma region callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#pragma endregion

void readInput(GLFWwindow* window, std::vector<bool>& input)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    switch (glfwGetKey(window, GLFW_KEY_SPACE))
    {
    case GLFW_PRESS:
        input[0] = true;
        break;
    case GLFW_RELEASE:
        input[0] = false;
        break;
    default:
        ASSERT(false);
        break;
    }
}

void update(GLFWwindow* window, std::vector<GameObject*>& gObjs, std::vector<GameObject*>& destroyedObjs)
{
    std::vector<bool> input{false};
    readInput(window, input);

    for (auto gObj : gObjs)
    {
        std::cout << "Update" << std::endl;
        gObj->Update(input);
    }
    for (auto gObj : destroyedObjs)
    {
        auto it = std::find(gObjs.begin(), gObjs.end(), gObj);
        gObjs.erase(it);
    }
    destroyedObjs.clear();
}

void increaseFPS()
{
    if (FRAMES_PER_SECOND == 3)
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
    
    std::vector<GameObject*> gObjs;
    std::vector<GameObject*> destroyedObjs;
    Player player(ShapeDict::Shape::rect, gObjs, destroyedObjs);
    player.SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    gObjs.push_back(&player);


#pragma region va->vb->ib->shader

    // Shader
    Shader shader("Resource/Shader/basic.shader");
    shader.Bind();

    // Unbind
    shader.Unbind();
#pragma endregion

    // FPS lock
    DWORD next_game_tick = GetTickCount();

    int sleep_time = 0;

    for (auto gObj : gObjs)
    {
        gObj->Start();
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Update here */
        update(window, gObjs, destroyedObjs);

        /* Render here */
        glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.Bind();
        shader.SetUniformMat4f("projection", projection);
        shader.SetUniformMat4f("view", view);



        for (auto gObj : gObjs)
        {
            shader.SetUniformMat4f("model", player.modelMatrix);
            gObj->va.Bind();
            gObj->vb.Bind();
            gObj->ib.Bind();
            glDrawElements(GL_TRIANGLES, gObj->shapeInfo.indices.size(), GL_UNSIGNED_INT, 0);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        // Sleep to wait for tick
        next_game_tick += SKIP_TICKS;
        sleep_time = next_game_tick - GetTickCount();
        if (sleep_time >= 0)
        {
            Sleep(sleep_time);
        }
        else
        {
            reduceFPS();
        }
    }

    glfwTerminate();
    return 0;
}