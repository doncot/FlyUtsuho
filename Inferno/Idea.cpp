#include<GameElements.h>
#include"Common.h"
#include"Graphics.h"

namespace Inferno
{
Idea::Idea()
: m_tex(nullptr), m_width(0), m_height(0) {}

Idea::~Idea()
{}

void Idea::SetTexture(const Texture& tex)
{
	m_tex = &tex;
	//•`‰æŠî€‚ÌŒvŽZ
	m_width = m_tex->GetWidth();
	m_height = m_tex->GetHeight();

	m_drawBase.x = m_width / 2;
	m_drawBase.y = m_height / 2;
}

const Texture* Idea::GetTexture() const
{
	return m_tex;
}

Vec2<int> Idea::GetDrawBase() const
{
	return m_drawBase;
}

int Idea::GetWidth() const
{
	return m_width;
}

int Idea::GetHeight() const
{
	return m_height;
}

}