#ifndef TEXTURESHADER_H_
#define TEXTURESHADER_H_

#include "Shader.h"

namespace Framework
{
	class Texture;

	class TextureShader
		:	public Shader
	{
	private:
		Texture*	m_pTexture;
		GLint		m_positionAttributeHandle;
		GLint		m_texCoordAttributeHandle;
		GLint		m_samplerHandle;

	public:
		TextureShader();
		virtual ~TextureShader();

		virtual void Link();
		virtual void Setup(Renderable& renderable);

		void			SetTexture(Texture* pTexture);
		Texture*		GetTexture();
	};

	inline void TextureShader::SetTexture(Texture* pTexture)
	{
		m_pTexture = pTexture;
	}

	inline Texture* TextureShader::GetTexture()
	{
		return m_pTexture;
	}
}

#endif // TEXTURESHADER_H_
