#pragma once
#include "util.h"

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size, GLenum usage);
	~VertexBuffer();

	void Update(const void* data, unsigned int size, GLenum usage);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RenderedID;
};