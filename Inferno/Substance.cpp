#include<GameElements.h>
#include"Graphics.h"

namespace Inferno
{
Substance::Substance() : m_idea(nullptr), m_pos(Vec2<int>(0, 0)), m_attribute(0){}
Substance::Substance(Idea* idea) : m_idea(idea), m_pos(Vec2<int>(0, 0)),m_attribute(0)
{
	SetAttribute(GEAttribute::ge_draw, true);
}
Substance::~Substance() {}

void Substance::SetIdea(Idea* idea)
{
	m_idea = idea;
	SetAttribute(GEAttribute::ge_draw, true);
}

void Substance::AMove(const int x, const int y)
{
	if (CheckAttribute(GEAttribute::ge_userLock)) return;
	m_pos.x = x;
	m_pos.y = y;
}

void Substance::AMove(const Vec2<int>& c)
{
	if (CheckAttribute(GEAttribute::ge_userLock)) return;
	AMove(c.x, c.y);
}

void Substance::RMove(const int x, const int y)
{
	if (CheckAttribute(GEAttribute::ge_userLock)) return;
	m_pos.x += x;
	m_pos.y += y;
}

void Substance::RMove(const Vec2<int>& c)
{
	if (CheckAttribute(GEAttribute::ge_userLock)) return;
	RMove(c.x, c.y);
}

//������W�ňʒu���w��
void Substance::SetPosofULCorner(const int x, const int y)
{
	if (CheckAttribute(GEAttribute::ge_userLock)) return;
	m_pos.x = x + m_idea->GetWidth()/2;
	m_pos.y = y + m_idea->GetHeight()/2;
}

void Substance::Rotate(const float angle)
{
	m_angle = angle;
}

Vec2<int> Substance::GetPosition() const
{
	return m_pos;
}

Rect Substance::GetRegion() const
{
	Rect r(m_pos, m_idea->GetHeight(), m_idea->GetWidth());
	return r;
}

bool Substance::CheckAttribute(GEAttribute attr) const
{
	return (m_attribute & attr) ? true : false;
}

void Substance::SetAttribute(GEAttribute attr, bool value)
{
	//�r�b�g�𗧂Ă�ꍇ
	if (value)
	{
		m_attribute |= attr;
	}
	//�r�b�g���~�낷�ꍇ
	else
	{
		m_attribute &= ~attr;
	}
}


void Substance::Update() {}

void Substance::Draw(const Graphics& g) const
{
	//�`���ԂłȂ��ꍇ�A�A��
	if (!CheckAttribute(GEAttribute::ge_draw)) return;

	D3DXMATRIX matWorld; //����������o�ɂ���Ώȗ��ł��邪���Ă���
	D3DXMatrixIdentity(&matWorld);
	//�ړ�
	//��������s�N�Z�����W(int)���X�N���[�����W(float)�֕ϊ�������
	matWorld._41 = static_cast<float>(m_pos.x);
	matWorld._42 = static_cast<float>(m_pos.y);
	g.GetSprite()->SetTransform(&matWorld);
	//��]


	//�����ŃA���t�@���E�E�E�H


	g.DrawSprite(m_idea->GetTexture()->GetTexture(),
		Vec2<float>( static_cast<float>(m_idea->GetDrawBase().x), static_cast<float>(m_idea->GetDrawBase().y) )
		);
}

Substance& Substance::operator=(const Substance& s)
{
	//�������g������\�����l������
	if (&s != this)
	{
		m_pos = s.m_pos;
		//�C�f�A�͓����i�萔�Ȃ̂ɒ��Ӂj
	}
	return *this;
}

}