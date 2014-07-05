#include<GameElements.h>
#include"Graphics.h"

namespace Inferno
{
Substance::Substance() : m_idea(nullptr), m_pos(Vec2<int>(0, 0)), m_isActive(true){}
Substance::Substance(Idea* idea) : m_idea(idea), m_pos(Vec2<int>(0, 0)),m_isActive(true){}
Substance::~Substance() {}

void Substance::SetIdea(Idea* idea)
{
	m_idea = idea;
}

void Substance::AMove(const int x, const int y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void Substance::AMove(const Vec2<int>& c)
{
	AMove(c.x, c.y);
}

void Substance::RMove(const int x, const int y)
{
	m_pos.x += x;
	m_pos.y += y;
}

void Substance::RMove(const Vec2<int>& c)
{
	m_pos += c;
}

//������W�ňʒu���w��
void Substance::SetPosofULCorner(const int x, const int y)
{
	m_pos.x = x + m_idea->GetWidth()/2;
	m_pos.y = y + m_idea->GetHeight()/2;
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

bool Substance::isActive() const { return m_isActive; }

void Substance::Activate() { m_isActive = true; }

void Substance::Deactivate() { m_isActive = false; }

void Substance::Update() {}

void Substance::Draw(const Graphics& g) const
{
	D3DXMATRIX matWorld; //����������o�ɂ���Ώȗ��ł��邪���Ă���
	D3DXMatrixIdentity(&matWorld);
	//��������s�N�Z�����W(int)���X�N���[�����W(float)�֕ϊ�������
	matWorld._41 = static_cast<float>(m_pos.x);
	matWorld._42 = static_cast<float>(m_pos.y);
	g.GetSprite()->SetTransform(&matWorld);

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