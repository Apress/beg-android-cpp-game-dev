#ifndef TRANSFORMAMBIENTDIFFUSESHADER_H_
#define TRANSFORMAMBIENTDIFFUSESHADER_H_

#include "../../Math/Matrix4.h"
#include "Shader.h"

namespace Framework
{
	class TransformAmbientDiffuseShader
		:	public Shader
	{
	private:
		GLint		m_modelViewProjUniformHandle;
		GLint		m_modelITMatrixUniformHandle;
		GLint		m_positionAttributeHandle;
		GLint		m_normalAttributeHandle;
		GLint		m_ambientColorUniformHandle;
		GLint		m_diffuseColorUniformHandle;
		GLint		m_ambientLightUniformHandle;
		GLint		m_diffuseLightUniformHandle;
		GLint		m_lightDirectionUniformHandle;

	public:
		TransformAmbientDiffuseShader();
		virtual ~TransformAmbientDiffuseShader();

		virtual void Link();
		virtual void Setup(Renderable& renderable);
	};
}

#endif // TRANSFORMAMBIENTDIFFUSESHADER_H_
