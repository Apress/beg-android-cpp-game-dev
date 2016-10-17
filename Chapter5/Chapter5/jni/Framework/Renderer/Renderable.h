#ifndef RENDERABLE_H_
#define RENDERABLE_H_

namespace Framework
{
	class Geometry;
	class Shader;
	class Texture;

	class Renderable
	{
	private:
		Geometry*	m_pGeometry;
		Shader*		m_pShader;

	public:
		Renderable();
		~Renderable();

		void			SetGeometry(Geometry* pGeometry);
		Geometry*		GetGeometry();

		void			SetShader(Shader* pShader);
		Shader*			GetShader();
	};

	inline Renderable::Renderable()
		:	m_pGeometry(NULL)
		,	m_pShader(NULL)
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
