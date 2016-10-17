#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "../Math/Transform.h"
#include "Materials/Material.h"

namespace Framework
{
	class Geometry;
	class Shader;

	class Renderable
	{
	private:
		Geometry*	m_pGeometry;
		Material*	m_pMaterial;
		Transform	m_transform;

		Vector3		m_min;
		Vector3		m_max;
		bool		m_useBounds;

	public:
		Renderable();
		~Renderable();

		void			SetGeometry(Geometry* pGeometry);
		Geometry*		GetGeometry();

		void			SetMaterial(Material* pMaterial);
		Material*		GetMaterial();

		Transform&		GetTransform()	{ return m_transform; }

		void			SetBoundMin(const Vector3& min)	{ m_min = min; }
		const Vector3&	GetBoundMin() const				{ return m_min; }

		void			SetBoundMax(const Vector3& max)	{ m_max = max; }
		const Vector3&	GetBoundMax() const				{ return m_max; }

		void			SetUseBounds(bool enabled)		{ m_useBounds = enabled; }
		bool			GetUseBounds() const			{ return m_useBounds; }

		bool			IsInitialized() const { return m_pGeometry && m_pMaterial; }
	};

	inline Renderable::Renderable()
		:	m_pGeometry(NULL)
		,	m_pMaterial(NULL)
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

	inline void Renderable::SetMaterial(Material* pMaterial)
	{
		m_pMaterial = pMaterial;
	}

	inline Material* Renderable::GetMaterial()
	{
		return m_pMaterial;
	}
}

#endif // RENDERABLE_H_
