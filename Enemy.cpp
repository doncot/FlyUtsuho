#include"ShooterElements.h"
#include<cassert>

#define PI 3.14159265358979323846

namespace
{
	double SinWaveMotion(Millisec count, double period, double amplitude)
	{
		//sin( PI*2 / ���� * Count ) * �U��
		return amplitude * std::sin(PI * 2 / period * count);
	}
}

namespace Inferno
{
	Enemy::Enemy(){}

	Enemy::Enemy(const Idea& idea)
	{
		this->SetIdea(idea);
	}

	void Enemy::Entry()
	{
		//�X�^���o�C��ԂłȂ��Ȃ�A��
		if (m_curState != EState::e_standby) return;

		m_curState = EState:: e_entry;
		m_timer.Start();
		SetPosofULCorner(700, 100);
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
			m_curState = EState::e_neutral;
			break;

		case EState::e_neutral:
			this->AMove(this->GetPosition().x,
				static_cast<const int>(230 + SinWaveMotion(m_timer.GetElapsed(), 2000, 180))
				);

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
			assert(0);
			break;
		}
	}



}