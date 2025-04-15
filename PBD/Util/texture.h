#pragma once

#include "util.h"

class Texture
{
public:
	~Texture();

	unsigned int AddTexture(const std::string& path);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth(int index) const { return m_Widths[index]; }
	inline int GetHeight(int index) const { return m_Heights[index]; }

private:
	std::unordered_map<std::string, unsigned int> loadedTexture;
	std::vector<unsigned int> m_RendererIDs;
	std::vector<std::string> m_FilePaths;
	unsigned char* m_LocalBuffer;
	std::vector<int> m_Widths, m_Heights, m_BPPs;
};