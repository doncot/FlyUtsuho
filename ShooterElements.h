#pragma once

#include<GameElements.h>
#include"MyTypes.h"
#include<Timer.h>

namespace Inferno
{
class Bullet : public Substance
{
public:
	Bullet(Inferno::Idea* idea) : Substance(idea) {}

	void Fire(const Inferno::Vec2<int>& bornPos, const int x, const int y)
	{
		this->AMove(bornPos.x + m_idea->GetWidth(), bornPos.y + m_idea->GetHeight() / 2);

		m_vel.x = x;
		m_vel.y = y;
	}

	void Fire(const Inferno::Vec2<int>& bornPos, const Inferno::Vec2<int>& vel)
	{
		this->Fire(bornPos, vel.x, vel.y);
	}

	//Nway�e�����
	void Nway(const Inferno::Vec2<int>& bornPos, const int angle);

	//�A�j���[�V�����Ƃ��ړ��Ƃ�
	void Update()
	{
		this->RMove(m_vel);
	}

private:
	typedef Inferno::Substance Base;
	Inferno::Vec2<int> m_vel;

};

enum class SEState
{
	se_neutral,
	se_damage
};
//enum class SEAnimeState;

class Player : public Substance
{
public:
	typedef Substance Base;

	Player() : m_curState(SEState::se_neutral),m_moveLimit(0,0) {}

	//���ΕW�w��ړ��i�����I�ɂ͎��g�ňړ����Ǘ��������j
	void RMove(const int x, const int y);
	void RMove(const Vec2<int>& c);

	void SetMoveLimit(const Rect& rect);

	void Hit();
	void Update();

private:
	//�A�j���[�V�����p�̏��
	SEState m_curState;
	Stopwatch m_timer;

	Rect m_moveLimit;

	//�萔
	const int HitBlowDistance = 20;
	Inferno::Vec2<int> hitStartPoint;
};

class Enemy : public Substance
{
public:

private:
	
};

}