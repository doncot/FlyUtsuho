#pragma once

#include<list>
#include<GameElements.h>
#include"MyTypes.h"
#include<Timer.h>
#include"Animation.h"
#include"ResourceManager.h"

namespace Inferno
{
//後ろにあるので
class Enemy;

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

//弾を持つクラス用に関数を提供
class BulletOwner
{
public:
	//自身が管理する弾が、指定対象に当たったか判定
	bool CheckBulletHit(const Rect& hitbox) const;
	//自分が撃った弾を渡す（読み取り専用） (死んだとき委譲する奴も別に作る必要があるな)
	list<Bullet*> GetBulletList() const { return m_bullets; }
	void EraseGivenBullet(const Bullet& bullet);

protected:
	//弾を一発発射する（上位でこれを呼ぶ）
	//角度を指定
	void ShootInternal(const Vec2<int> bornPos, const float degree, const int speed);

	//自身が管理する弾のリスト
	std::list<Bullet*> m_bullets;
};

enum class PState
{
	Neutral,
	Damaged,
};
//enum class SEAnimeState;

class Player : public Substance, public BulletOwner
{
public:
	Player() : m_curState(PState::Neutral),m_moveLimit(0,0) {}
	~Player();

	//相対標指定移動（将来的には自身で移動を管理したい）
	void RMove(const int x, const int y);
	void RMove(const Vec2<int>& c);

	void SetMoveLimit(const Rect& rect);

	//弾・敵に当たったら（要改修）
	void ProcessHit();

	void Shoot(const float degree, const int speed);

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
};

enum class EState
{
	Standby,
	Neutral,
	//Damage,
	Entry,
	Leave,
};

class Enemy : public Substance, public BulletOwner
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