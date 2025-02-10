#include "Texture.h"
#include "stb_image.h"


Texture::~Texture()
{
	for (auto& m_RendererID : m_RendererIDs)
	{
		glDeleteTextures(1, &m_RendererID);
	}
	m_RendererIDs.clear();
}

unsigned int Texture::AddTexture(const std::string& path)
{
	m_LocalBuffer = nullptr;
	if (loadedTexture.find(path) != loadedTexture.end())
	{
		return loadedTexture[path];
	}

	unsigned int i = m_RendererIDs.size();
	m_RendererIDs.push_back(0);
	m_Widths.push_back(0);
	m_Heights.push_back(0);
	m_BPPs.push_back(0);

	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Widths[i], &m_Heights[i], &m_BPPs[i], 4);

	if (!m_LocalBuffer)
	{
		std::cout << "Failed to load texture at: " << path << std::endl;
		ASSERT(false);
	}
	std::cout << "Finish loading slot " << i << " texture at : " << path << std::endl;

	glGenTextures(1, &m_RendererIDs[i]);
	glBindTexture(GL_TEXTURE_2D, m_RendererIDs[i]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Widths[i], m_Heights[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
	loadedTexture[path] = i;
	Bind(i);
	return i;
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererIDs[slot]);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}