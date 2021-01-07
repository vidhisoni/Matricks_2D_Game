/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	m_Stride = 0;
	m_Offset = 0;
	m_Index  = 0;
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
}

void VertexArray::AddLayout()
{
	for (auto c : m_Elements)
	{
		GLCall(glVertexAttribPointer(m_Index, c.m_Count, c.m_Type, GL_FALSE, m_Stride, (const void*)m_Offset));
		GLCall(glEnableVertexAttribArray(m_Index));
		m_Offset += c.m_Size * c.m_Count;
		m_Index++;
	}
}

void VertexArray::Push(unsigned int count, unsigned int type, unsigned int size)
{
	VertexElements element(count, type, size);
	m_Elements.push_back(element);
	m_Stride += count * size;

}

void VertexArray::AddBuffer(const VertexBuffer& vb) const
{
	Bind();
	vb.Bind();
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::Delete() const
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

VertexElements::VertexElements(unsigned int count, unsigned int type, unsigned int size) :m_Count(count), m_Type(type), m_Size(size)
{
}