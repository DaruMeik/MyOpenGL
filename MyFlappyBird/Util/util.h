#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <unordered_map>

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define ASSERT(x) if(!(x)) __debugbreak();