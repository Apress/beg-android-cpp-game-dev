#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "../Math/Transform.h"
#include "../Math/Vector4.h"

namespace Framework
{
	class Geometry;
	class Shader;

	class Renderable
	{
	private:
		Geometry*	m_pGeometry;
		Shader*		m_pShader;
		Transform	m_transform;
		Vector4		m_color;

	public:
		Renderable();
		~Renderable();

		void			SetGeometry(Geometry* pGeometry);
		Geometry*		GetGeometry();

		void			SetShader(Shader* pShader);
		Shader*			GetShader();

		Transform&		GetTransform()	{ return m_transform; }
		Vector4&		GetColor() 		{ return m_color; }

		bool			IsInitialized() const { return m_pGeometry && m_pShader; }
	};

	inline Renderable::Renderable()
		:	m_pGeometry(NULL)
		,	m_pShader(NULL)
		,	m_color(1.0f, 1.0f, 1.0f, 1.0f)
	{

	}

	inline Renderable::~Renderable()
	{

	}

	inline void Renderable::SetGeometry(Geometry* pGeometry)
	{
		m_pGeometry = pGeometry;
	}

	inline Geometry* Renderable::GetGeometry()
	{
		return m_pGeometry;
	}

	inline void Renderable::SetShader(Shader* pShader)
	{
		m_pShader = pShader;
	}

	inline Shader* Renderable::GetShader()
	{
		return m_pShader;
	}
}

#endif // RENDERABLE_H_
