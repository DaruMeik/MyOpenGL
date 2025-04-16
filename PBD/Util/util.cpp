#include "util.h"

std::vector<std::string> split(const std::string& s, char delim)
{
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim))
	{
		result.push_back(item);
	}

	return result;
}

bool loadOBJ(
	const std::string& filepath,
	std::vector < GLfloat >& out_vertices,
	std::vector < unsigned int >& out_indices
)
{
	std::vector< unsigned int > vertexIndices, normalIndices;
	std::unordered_map<std::string, unsigned int> map;
	unsigned int index = 0;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec3 > temp_normals;

	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss[2];
	if (!stream)
	{
		std::cout << "Failed to load obj at " << filepath << "!" << std::endl;
		return false;
	}

#pragma region reading file
	while (true)
	{
		char lineHeader[128];
		// read the first word of the line
		if (!(stream >> lineHeader))
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			stream >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			stream >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string data[3];
			std::vector<std::string> vertex[3];
			unsigned int indice[3];

			stream >> data[0] >> data[1] >> data[2];

			for (int i = 0; i < 3; i++)
			{
					map[data[i]] = index;
					index++;
				indice[i] = map[data[i]];
				vertex[i] = split(data[i], '/');
				out_indices.push_back(indice[i]);
			}

			vertexIndices.push_back(stoi(vertex[0][0]));
			vertexIndices.push_back(stoi(vertex[1][0]));
			vertexIndices.push_back(stoi(vertex[2][0]));
			normalIndices.push_back(stoi(vertex[0][2]));
			normalIndices.push_back(stoi(vertex[1][2]));
			normalIndices.push_back(stoi(vertex[2][2]));
		}
	}
#pragma endregion

#pragma region processing data
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int index[2];
		index[0] = vertexIndices[i];
		index[1] = normalIndices[i];
		//std::cout << i << ": " << index[0] << " " << index[1] << std::endl;
		glm::vec3 vertex = temp_vertices[index[0] - 1]; // obj index start at 1
		glm::vec3 normal = temp_normals[index[1] - 1];

		out_vertices.push_back(vertex.x);
		out_vertices.push_back(vertex.y);
		out_vertices.push_back(vertex.z);

		out_vertices.push_back(normal.x);
		out_vertices.push_back(normal.y);
		out_vertices.push_back(normal.z);
	}
#pragma endregion

	// Debug
	//std::cout << "DEBUG" << std::endl;
	//int t = 0;
	//for (auto c : out_vertices)
	//{
	//	std::cout << c << " ";
	//	t++;
	//	if (t % 6 == 0)
	//		std::cout << std::endl;
	//}
	//std::cout << std::endl;
	//t = 0;
	//for (auto c : out_indices)
	//{
	//	std::cout << c << " ";
	//	t++;
	//	if (t % 3 == 0)
	//		std::cout << std::endl;
	//}
	//std::cout << "--DEBUG--" << std::endl;

	return true;
}