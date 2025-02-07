#pragma once
#include "util.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind();
	void Unbind();
private:
	unsigned int m_RenderedID;
};