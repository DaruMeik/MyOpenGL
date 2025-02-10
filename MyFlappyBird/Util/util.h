#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <Windows.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <unordered_map>

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <stack>
#include <queue>

#include "stb_image.h"

#define ASSERT(x) if(!(x)) __debugbreak();