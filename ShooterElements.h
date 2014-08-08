#pragma once

#include<list>
#include<GameElements.h>
#include"MyTypes.h"
#include<Timer.h>
#include"Animation.h"
#include"ResourceManager.h"

namespace Inferno
{
class Bullet : public Substance
{
public:
	Bullet(const Inferno::Idea& idea) : Substance(idea) {}

	void Fire(const Inferno::Vec2<int>& bornPos, const int vel_x, const int vel_y)
	{
		this->AMove(bornPos.x + m_idea->GetWidth(), bornPos.y + m_idea->GetHeight() / 2);

		m_vel.x = vel_x;
		m_vel.y = vel_y;
	}

	void Fire(const Inferno::Vec2<int>& bornPos, const Inferno::Vec2<int>& vel)
	{
		this->Fire(bornPos, vel.x, vel.y);
	}
	
	//アニメーションとか移動とか
	void Update();

private:
	typedef Inferno::Substance Base;
	Inferno::Vec2<int> m_vel;
};

enum class PState
{
	Neutral,
	Damaged,
};
//enum class SEAnimeState;

class Player : public Substance
{
public:
	typedef Substance Base;

	Player() : m_curState(PState::Neutral),m_moveLimit(0,0) {}
	~Player();

	//相対標指定移動（将来的には自身で移動を管理したい）
	void RMove(const int x, const int y);
	void RMove(const Vec2<int>& c);

	void SetMoveLimit(const Rect& rect);

	//弾を一発発射する
	//角度を指定
	void Shoot(const float degree, const int speed);

	void Hit();
	void Update();
	void Draw(const Graphics& g) const;

private:
	//アニメーション用の状態
	PState m_curState;
	Stopwatch m_timer;

	Rect m_moveLimit;

	//定数
	const int HitBlowDistance = 20;
	Inferno::Vec2<int> hitStartPoint;
	//自身が管理する弾のリスト
	std::list<Bullet*> m_bullets;
};

enum class EState
{
	Standby,
	Neutral,
	//Damage,
	Entry,
	Leave,
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
	EState m_curState = EState::Standby;
	Animation m_entryAnime[2];
	Animation m_anime[2];
	Animation m_exitAnime[2];

	//定数
	const Millisec ShootInterval = 800;
};

}