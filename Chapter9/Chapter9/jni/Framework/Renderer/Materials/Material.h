#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../../Math/Vector4.h"

namespace Framework
{
	class Shader;
	class Texture;

	class Material
	{
	private:
		Shader*		m_pShader;
		Texture*	m_pTexture;

		Vector4		m_ambientColor;
		Vector4		m_diffuseColor;
		Vector4		m_specularColor;
		float		m_specularExponent;

	public:
		Material()
			:	m_pShader(NULL)
			,	m_pTexture(NULL)
			,	m_specularExponent(0.0f)
		{

		}
		~Material()
		{

		}

		void			SetShader(Shader* pShader)					{ m_pShader = pShader; }
		Shader*			GetShader() const							{ return m_pShader; }

		void			SetTexture(Texture* pTexture)				{ m_pTexture = pTexture; }
		Texture*		GetTexture() const							{ return m_pTexture; }

		void			SetAmbientColor(Vector4 ambientColor)		{ m_ambientColor = ambientColor; }
		const Vector4&	GetAmbientColor() const						{ return m_ambientColor; }

		void			SetDiffuseColor(Vector4 diffuseColor)		{ m_diffuseColor = diffuseColor; }
		const Vector4&	GetDiffuseColor() const						{ return m_diffuseColor; }

		void			SetSpecularColor(Vector4 specularColor)		{ m_specularColor = specularColor; }
		const Vector4&	GetSpecularColor() const					{ return m_specularColor; }

		void			SetSpecularExponent(float specularExponent)	{ m_specularExponent = specularExponent; }
		const float		GetSpecularExponent() const					{ return m_specularExponent; }
	};
}

#endif // MATERIAL_H_
