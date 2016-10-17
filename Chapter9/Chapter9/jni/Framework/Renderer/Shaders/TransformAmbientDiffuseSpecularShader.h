#ifndef TRANSFORMAMBIENTDIFFUSESPECULARSHADER_H_
#define TRANSFORMAMBIENTDIFFUSESPECULARSHADER_H_

#include "../../Math/Matrix4.h"
#include "Shader.h"

namespace Framework
{
	class TransformAmbientDiffuseSpecularShader
		:	public Shader
	{
	private:
		GLint		m_modelViewProjUniformHandle;
		GLint		m_modelITMatrixUniformHandle;
		GLint		m_positionAttributeHandle;
		GLint		m_normalAttributeHandle;
		GLint		m_ambientColorUniformHandle;
		GLint		m_diffuseColorUniformHandle;
		GLint		m_specularColorUniformHandle;
		GLint		m_specularExponentUniformHandle;
		GLint		m_ambientLightUniformHandle;
		GLint		m_diffuseLightUniformHandle;
		GLint		m_specularLightUniformHandle;
		GLint		m_lightDirectionUniformHandle;
		GLint		m_lightHalfVectorUniformHandle;

	public:
		TransformAmbientDiffuseSpecularShader();
		virtual ~TransformAmbientDiffuseSpecularShader();

		virtual void Link();
		virtual void Setup(Renderable& renderable);
	};
}

#endif // TRANSFORMAMBIENTDIFFUSESPECULARSHADER_H_
