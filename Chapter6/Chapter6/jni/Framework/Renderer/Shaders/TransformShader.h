#ifndef TRANSFORMSHADER_H_
#define TRANSFORMSHADER_H_

#include "../../Math/Matrix4.h"
#include "Shader.h"

namespace Framework
{
	class TransformShader
		:	public Shader
	{
	private:
		Matrix4		m_projection;

		GLint		m_transformUniformHandle;
		GLint		m_positionAttributeHandle;
		GLint		m_colorAttributeHandle;

	public:
		TransformShader();
		virtual ~TransformShader();

		virtual void Link();
		virtual void Setup(Renderable& renderable);
	};
}

#endif // TRANSFORMSHADER_H_
