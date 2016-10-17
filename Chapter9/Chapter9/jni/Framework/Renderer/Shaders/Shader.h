#ifndef SHADER_H_
#define SHADER_H_

#include <GLES2/gl2.h>
#include <string>
#include "../Renderable.h"

namespace Framework
{
	class Shader
	{
	private:
		void LoadShader(GLenum shaderType, std::string& shaderCode);

	protected:
		GLuint			m_vertexShaderId;
		GLuint			m_fragmentShaderId;
		GLint			m_programId;

		std::string		m_vertexShaderCode;
		std::string		m_fragmentShaderCode;

		bool			m_isLinked;

	public:
		Shader();
		virtual ~Shader();

		virtual void Link();
		virtual void Setup(Renderable& renderable);

		bool IsLinked() { return m_isLinked; }
	};
}

#endif // SHADER_H_
