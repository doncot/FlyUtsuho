#include<MyTypes.h>

namespace Inferno
{
Rect::Rect() :m_height(0),m_width(0){}

Rect::Rect(const int height, const int width)
{
	SetSize(height, width);
}

Rect::Rect(const Vec2<int>& p, const int height, const int width)
{
	m_center = p;
	SetSize(height, width);
}

//Win32‚ÌRECT‚ð•Ô‚·
RECT Rect::GetMSRECT() const
{
	RECT r;
	r.top = m_center.y - m_height / 2;
	r.left = m_center.x - m_width / 2;
	r.right = m_center.x + m_width / 2;
	r.bottom = m_center.y + m_height / 2;
	return r;
}

Vec2<int> Rect::Center() const { return m_center; }
int Rect::Top() const {	return m_center.y - m_height / 2; }
int Rect::Left() const { return m_center.x - m_width / 2; }
int Rect::Right() const { return m_center.x + m_width / 2; }
int Rect::Bottom() const { return m_center.y + m_height / 2; }
int Rect::Height() const { return m_height; }
int Rect::Width() const { return m_width; }

Vec2<int> Rect::UpperLeft() const {	return Vec2<int>(Left(), Top()); }
Vec2<int> Rect::UpperRight() const { return Vec2<int>(Right(), Top()); }
Vec2<int> Rect::BottomLeft() const { return Vec2<int>(Left(), Bottom()); }
Vec2<int> Rect::BottomRight() const { return Vec2<int>(Right(), Bottom()); }

void Rect::SetSize(const int height, const int width)
{
	m_height = height;
	m_width = width;
}

void Rect::AMove(const Vec2<int>& v)
{
	m_center = v;
}

void Rect::AMove(const int x, const int y)
{
	m_center.x = x;
	m_center.y = y;
}

void Rect::RMove(const int x, const int y)
{
	m_center.x += x;
	m_center.y += y;
}

void Rect::SetPosofULCorner(const int x, const int y)
{
	m_center.x = x + Width() / 2;
	m_center.y = y + Height() / 2;
}

}