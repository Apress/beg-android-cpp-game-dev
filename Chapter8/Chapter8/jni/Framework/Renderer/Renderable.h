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

		Vector3		m_min;
		Vector3		m_max;
		bool		m_useBounds;

	public:
		Renderable();
		~Renderable();

		void			SetGeometry(Geometry* pGeometry);
		Geometry*		GetGeometry();

		void			SetShader(Shader* pShader);
		Shader*			GetShader();

		Transform&		GetTransform()	{ return m_transform; }
		Vector4&		GetColor() 		{ return m_color; }

		void			SetBoundMin(const Vector3& min)	{ m_min = min; }
		const Vector3&	GetBoundMin() const				{ return m_min; }

		void			SetBoundMax(const Vector3& max)	{ m_max = max; }
		const Vector3&	GetBoundMax() const				{ return m_max; }

		void			SetUseBounds(bool enabled)		{ m_useBounds = enabled; }
		bool			GetUseBounds() const			{ return m_useBounds; }

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
