#include "Chapter5Task.h"
#include "Framework/Renderer/Renderer.h"
#include "Framework/Utilities/TGAFile.h"

namespace
{
	float verts[] =
	{
			-0.5f, 0.5f, 0.0f,
			0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,
			1.0f, 1.0f,
			-0.5f, -0.5f, 0.0f,
			0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,
			1.0f, 0.0f
	};

	unsigned short indices[] =
	{
			0,	2,	1,	2,	3,	1
	};

}

Chapter5Task::Chapter5Task(Framework::Renderer* pRenderer, const unsigned int priority)
	:	m_pRenderer(pRenderer)
	,	Framework::Task(priority)
	,	m_state(RUNNING)
	,	m_file("test.tga")
	,	m_pTGABuffer(NULL)
	,	m_readBytes(0)
{
	m_renderable.SetGeometry(&m_geometry);
	m_renderable.SetShader(&m_textureShader);
}

Chapter5Task::~Chapter5Task()
{
	m_pRenderer->RemoveRenderable(&m_renderable);
	m_pRenderer->RemoveTexture(&m_texture);
	m_pRenderer->RemoveShader(&m_textureShader);

	if (m_pTGABuffer)
	{
		delete[] static_cast<char*>(m_pTGABuffer);
		m_pTGABuffer = 0;
	}
}

bool Chapter5Task::Start()
{
	Framework::Geometry* pGeometry = m_renderable.GetGeometry();
	pGeometry->SetVertexBuffer(verts);
	pGeometry->SetNumVertices(4);
	pGeometry->SetIndexBuffer(indices);
	pGeometry->SetNumIndices(6);
	pGeometry->SetName("quad");

	pGeometry->SetNumVertexPositionElements(3);
	pGeometry->SetNumTexCoordElements(2);
	pGeometry->SetVertexStride(sizeof(float) * 5);

	bool success = false;
	if (m_file.Open())
	{
		m_fileLength = m_file.Length();

		m_pTGABuffer = new char[m_fileLength];

		m_state = LOADING_FILE;
		success = true;
	}

	return success;
}

void Chapter5Task::OnSuspend()
{

}

void Chapter5Task::Update()
{
	switch (m_state)
	{
	case LOADING_FILE:
	{
		void* pCurrentDataPos = static_cast<char*>(m_pTGABuffer) + (sizeof(char) * m_readBytes);

		size_t bytesRead = 0;
		m_file.Read(pCurrentDataPos, 512 * 1024, bytesRead);

		m_readBytes += bytesRead;
		if (m_readBytes == m_fileLength)
		{
			m_state = CREATE_TEXTURE;
		}
	}
	break;

	case CREATE_TEXTURE:
	{
		Framework::TGAFile tgaFile(m_pTGABuffer);

		Framework::Texture::Header textureHeader;
		textureHeader.m_height = tgaFile.GetHeight();
		textureHeader.m_width = tgaFile.GetWidth();
		textureHeader.m_bytesPerPixel = 4;
		textureHeader.m_dataSize = textureHeader.m_height * textureHeader.m_width * textureHeader.m_bytesPerPixel;

		m_texture.SetData(textureHeader, tgaFile.GetImageData());

		m_pRenderer->AddShader(&m_textureShader);
		m_pRenderer->AddTexture(&m_texture);

		m_textureShader.SetTexture(&m_texture);

		m_pRenderer->AddRenderable(&m_renderable);

		m_state = RUNNING;
	}
	break;
	};
}

void Chapter5Task::OnResume()
{

}

void Chapter5Task::Stop()
{
	m_pRenderer->RemoveRenderable(&m_renderable);
}
