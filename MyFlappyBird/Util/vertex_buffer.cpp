#include "vertex_buffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum usage)
{
	glGenBuffers(1, &m_RenderedID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RenderedID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderedID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}