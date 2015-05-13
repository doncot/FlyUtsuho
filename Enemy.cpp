#include"ShooterElements.h"
#include<cassert>

#define PI 3.14159265358979323846

namespace
{
	double SinWaveMotion(Millisec count, double period, double amplitude)
	{
		//sin( PI*2 / ŽüŠú * Count ) * U•
		return amplitude * std::sin(PI * 2 / period * count);
	}
}

namespace Inferno
{
	typedef Substance Base;

	Enemy::Enemy()
	{
		this->SetAttribute(GEAttribute::Visible, false);
	}

	Enemy::Enemy(const Idea& idea)
	{
		this->SetIdea(idea);
		this->SetAttribute(GEAttribute::Visible, false);
	}

	void Enemy::Entry()
	{
		//ƒXƒ^ƒ“ƒoƒCó‘Ô‚Å‚È‚¢‚È‚ç‹A‚é
		if (m_curState != EState::Standby) return;

		m_curState = EState:: Entry;
		this->SetAttribute(GEAttribute::Visible, true);
		m_timer.Start();
		SetPosofULCorner(700, 100);
		m_entryAnime[0].Start(0, 1000, 900, 680, Animation::EaseOut);
		m_entryAnime[1].Start(0, 1000, 50, 150, Animation::EaseOut);
		m_anime[0].Set(100, 3000, 680, 320, Animation::Linear);
		m_exitAnime[0].Set(50, 1200, 320, -100, Animation::EaseIn);
		m_exitAnime[1].Set(50, 1200, 150, -100, Animation::EaseIn);
	}

	void Enemy::Update()
	{
		static Inferno::Stopwatch shootInterval(ShootInterval);

		switch (m_curState)
		{
		case EState::Standby:
			//do nothing
			break;

		case EState::Entry:
			this->AMove(m_entryAnime[0].GetValue(),
				m_entryAnime[1].GetValue()
				);
			
			if (m_entryAnime[0].HasEnded())
			{
				m_curState = EState::Neutral;
				m_anime[0].Start();
			}
			break;

		case EState::Neutral:
			/*
			this->AMove(this->GetPosition().x,
				static_cast<const int>(230 + SinWaveMotion(m_timer.GetElapsed(), 3000, 180))
				);
			*/
			this->AMove(m_anime[0].GetValue(), GetPosition().y);

			if (shootInterval.HasFinished())
			{
				ShootInternal(m_pos, 180, 1.0);
				/*
				auto born = new Inferno::Bullet(&purpleBullet_);
				born->Fire(m_onryouList[0]->GetPosition(), Inferno::Vec2<int>(-2, 0));
				purpleBullets.push_back(born);
				shootInterval.Restart();
				*/
			}

			if (m_anime[0].HasEnded())
			{
				m_curState = EState::Leave;
				m_exitAnime[0].Start();
				m_exitAnime[1].Start();
			}

			break;

		case EState::Leave:
			this->AMove(m_exitAnime[0].GetValue(), m_exitAnime[1].GetValue());

			break;
		default:
			//shoudn't come here
			assert(0);
			break;
		}
	}

	void Enemy::Draw(const Graphics& g) const
	{
		//Ž©•ªŽ©g‚ð•`‰æ
		Base::Draw(g);
		//’e‚Ì•`‰æ
		for (auto e : m_bullets)
		{
			e->Draw(g);
		}
	}

}