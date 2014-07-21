#pragma once

#include<GameElements.h>
#include"MyTypes.h"
#include<Timer.h>
#include"Animation.h"

namespace Inferno
{
class Bullet : public Substance
{
public:
	Bullet(const Inferno::Idea& idea) : Substance(idea) {}

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

	//Nway弾を放射
	void Nway(const Inferno::Vec2<int>& bornPos, const int angle);

	//アニメーションとか移動とか
	void Update()
	{
		this->RMove(m_vel);
	}

private:
	typedef Inferno::Substance Base;
	Inferno::Vec2<int> m_vel;

};

enum class PState
{
	p_neutral,
	p_damage,
};
//enum class SEAnimeState;

class Player : public Substance
{
public:
	typedef Substance Base;

	Player() : m_curState(PState::p_neutral),m_moveLimit(0,0) {}

	//相対標指定移動（将来的には自身で移動を管理したい）
	void RMove(const int x, const int y);
	void RMove(const Vec2<int>& c);

	void SetMoveLimit(const Rect& rect);

	void Hit();
	void Update();

private:
	//アニメーション用の状態
	PState m_curState;
	Stopwatch m_timer;

	Rect m_moveLimit;

	//定数
	const int HitBlowDistance = 20;
	Inferno::Vec2<int> hitStartPoint;
};

enum class EState
{
	e_standby,
	e_neutral,
	//e_damage,
	e_entry,
};

class Enemy : public Substance
{
public:
	Enemy();
	Enemy(const Idea& idea);

	void Entry();
	void Update();

private:
	Timer m_timer;
	EState m_curState = EState::e_standby;
	Animation m_entryAnime[2];
	Animation m_anime[2];

	//定数
	const Millisec ShootInterval = 800;
};

}