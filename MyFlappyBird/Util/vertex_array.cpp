#include "vertex_array.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RenderedID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RenderedID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
		glEnableVertexAttribArray(0);
		offset += VertexBufferElement::GetSizeOfType(element.type) * element.count;
	}
}

void VertexArray::Bind()
{
	glBindVertexArray(m_RenderedID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}