#ifndef TRANSFORMAMBIENTSHADER_H_
#define TRANSFORMAMBIENTSHADER_H_

#include "../../Math/Matrix4.h"
#include "Shader.h"

namespace Framework
{
	class TransformAmbientShader
		:	public Shader
	{
	private:
		GLint		m_transformUniformHandle;
		GLint		m_positionAttributeHandle;
		GLint		m_normalAttributeHandle;
		GLint		m_colorUniformHandle;
		GLint		m_ambientLightUniformHandle;

	public:
		TransformAmbientShader();
		virtual ~TransformAmbientShader();

		virtual void Link();
		virtual void Setup(Renderable& renderable);
	};
}

#endif // TRANSFORMAMBIENTSHADER_H_
