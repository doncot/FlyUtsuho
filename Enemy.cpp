#include"ShooterElements.h"
#include<cassert>

#define PI 3.14159265358979323846

namespace
{
	double SinWaveMotion(Millisec count, double period, double amplitude)
	{
		//sin( PI*2 / 周期 * Count ) * 振幅
		return amplitude * std::sin(PI * 2 / period * count);
	}
}

namespace Inferno
{
	Enemy::Enemy()
	{
		this->SetAttribute(GEAttribute::ge_draw, false);
	}

	Enemy::Enemy(const Idea& idea)
	{
		this->SetIdea(idea);
		this->SetAttribute(GEAttribute::ge_draw, false);
	}

	void Enemy::Entry()
	{
		//スタンバイ状態でないなら帰る
		if (m_curState != EState::e_standby) return;

		m_curState = EState:: e_entry;
		this->SetAttribute(GEAttribute::ge_draw, true);
		m_timer.Start();
		SetPosofULCorner(700, 100);
		m_entryAnime[0].Start(0, 1000, 900, 680, Animation::TT_EaseOut);
		m_entryAnime[1].Start(0, 1000, 50, 150, Animation::TT_EaseOut);
	}

	void Enemy::Update()
	{
		static Inferno::Stopwatch shootInterval(ShootInterval);

		switch (m_curState)
		{
		case EState::e_standby:
			//do nothing
			break;

		case EState::e_entry:
			this->AMove(m_entryAnime[0].GetValue(),
				m_entryAnime[1].GetValue()
				);
			
			if (m_entryAnime[0].HasEnded())
			{
				m_curState = EState::e_neutral;
				m_anime[0].Start(100, 3000, 680, 320, Animation::TT_Linear);
			}
			break;

		case EState::e_neutral:
			/*
			this->AMove(this->GetPosition().x,
				static_cast<const int>(230 + SinWaveMotion(m_timer.GetElapsed(), 3000, 180))
				);
			*/
			this->AMove(m_anime[0].GetValue(), GetPosition().y);

			if (shootInterval.HasFinished())
			{
				/*
				auto born = new Inferno::Bullet(&purpleBullet_);
				born->Fire(m_onryouList[0]->GetPosition(), Inferno::Vec2<int>(-2, 0));
				purpleBullets.push_back(born);
				shootInterval.Restart();
				*/
			}
			break;

		default:
			//shoudn't come here
			assert(0);
			break;
		}
	}



}