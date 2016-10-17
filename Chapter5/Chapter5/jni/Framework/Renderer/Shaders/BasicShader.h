#ifndef BASICSHADER_H_
#define BASICSHADER_H_

#include "Shader.h"

namespace Framework
{
	class BasicShader
		:	public Shader
	{
	private:
		GLint	m_positionAttributeHandle;

	public:
		BasicShader();
		virtual ~BasicShader();

		virtual void Link();
		virtual void Setup(Renderable& renderable);
	};
}

#endif // BASICSHADER_H_
