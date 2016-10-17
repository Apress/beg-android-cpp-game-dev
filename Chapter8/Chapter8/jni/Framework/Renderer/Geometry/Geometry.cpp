/*
* Geometry.cpp
*
*  Created on: 05/06/2012
*      Author: bruce
*/

#include "Geometry.h"

namespace Framework
{
	Geometry::Geometry()
		:	m_numVertices(0)
		,	m_numIndices(0)
		,	m_pVertices(NULL)
		,	m_pIndices(NULL)
		,	m_numVertexPositionElements(0)
		,	m_numTexCoordElements(0)
		,	m_vertexStride(0)
	{
	}

	Geometry::~Geometry()
	{
	}
} // Framework
