#pragma once
#include "util.h"

class IndexBuffer
{
public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();

	inline unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RenderedId;
	unsigned int m_Count;
};