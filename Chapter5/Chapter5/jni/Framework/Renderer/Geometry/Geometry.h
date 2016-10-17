#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <string>

namespace Framework
{
	class Geometry
	{
	private:
		static const unsigned int NAME_MAX_LENGTH = 16;

		char		m_name[NAME_MAX_LENGTH];
		int			m_numVertices;
		int			m_numIndices;
		void*		m_pVertices;
		void*		m_pIndices;

		int			m_numVertexPositionElements;
		int			m_numTexCoordElements;
		int			m_vertexStride;

	public:
		Geometry();
		virtual ~Geometry();

		void				SetName(const char* name)				{ strcpy(m_name, name); }
		void	 			SetNumVertices(const int numVertices)	{ m_numVertices = numVertices; }
		void	 			SetNumIndices(const int numIndices)		{ m_numIndices = numIndices; }

		const char*			GetName() const							{ return m_name; }
		const int 			GetNumVertices() const					{ return m_numVertices; }
		const int 			GetNumIndices() const					{ return m_numIndices; }

		void*				GetVertexBuffer() const					{ return m_pVertices; }
		void*				GetIndexBuffer() const 					{ return m_pIndices; }

		void				SetVertexBuffer(void* pVertices)		{ m_pVertices = pVertices; }
		void				SetIndexBuffer(void* pIndices)			{ m_pIndices = pIndices; }

		void				SetNumVertexPositionElements(const int numVertexPositionElements);
		int					GetNumVertexPositionElements() const	{ return m_numVertexPositionElements; }

		void				SetNumTexCoordElements(const int numTexCoordElements);
		int					GetNumTexCoordElements() const			{ return m_numTexCoordElements; }

		void				SetVertexStride(const int vertexStride)	{ m_vertexStride = vertexStride; }
		int					GetVertexStride() const					{ return m_vertexStride; }
	};

	inline void Geometry::SetNumVertexPositionElements(const int numVertexPositionElements)
	{
		m_numVertexPositionElements = numVertexPositionElements;
	}

	inline void Geometry::SetNumTexCoordElements(const int numTexCoordElements)
	{
		m_numTexCoordElements = numTexCoordElements;
	}
} // namespace Framework

#endif //GEOMETRY_H_
