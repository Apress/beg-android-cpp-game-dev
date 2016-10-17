#include "Shader.h"
#include <android/log.h>

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "Framework", __VA_ARGS__))

namespace Framework
{
	Shader::Shader()
		:	m_vertexShaderId(GL_INVALID_VALUE)
		,	m_fragmentShaderId(GL_INVALID_VALUE)
		,	m_programId(GL_INVALID_VALUE)
	{

	}

	Shader::~Shader()
	{

	}

	void Shader::LoadShader(GLuint id, std::string& shaderCode)
	{
		static const uint32_t NUM_SHADERS = 1;

		const GLchar* pCode = shaderCode.c_str();
		GLint length = shaderCode.length();

		glShaderSource(id, NUM_SHADERS, &pCode, &length);

		glCompileShader(id);

		GLint compiled = GL_FALSE;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE)
		{
			GLint logLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 1)
			{
				char log[logLength];
				glGetShaderInfoLog(id, logLength, NULL, log);
				LOGE("Error compiling shader: %s", log);
			}
		}

		glAttachShader(m_programId, id);
	}

	void Shader::Link()
	{
		m_programId = glCreateProgram();

		m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		LoadShader(m_vertexShaderId, m_vertexShaderCode);

		m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		LoadShader(m_fragmentShaderId, m_fragmentShaderCode);

		glLinkProgram(m_programId);

		m_isLinked = true;
	}

	void Shader::Setup(Renderable& renderable)
	{
		glUseProgram(m_programId);
	}
}
