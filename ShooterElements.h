#pragma once

#include<GameElements.h>
#include<MyTypes.h>
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

class ShooterActor : public Substance
{
public:
	void Hit();
	void Update();

private:
	//アニメーション用の状態
	GEState m_curState;
	Stopwatch m_timer;

	//定数
	const int HitBlowDistance = 20;
	Inferno::Vec2<int> hitStartPoint;
};

}