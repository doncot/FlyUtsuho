#include"ShooterElements.h"

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
	Enemy::Enemy()
	{
		m_timer.Start();
	}

	Enemy::Enemy(const Idea& idea)
	{
		m_timer.Start();
		this->SetIdea(idea);
	}

	void Enemy::Update()
	{
		this->AMove(this->GetPosition().x,
			static_cast<const int>(230+SinWaveMotion(m_timer.GetElapsed(), 2000, 180) )
			);

		static Inferno::Stopwatch shootInterval(ShootInterval);
		if (shootInterval.HasFinished())
		{
			/*
			auto born = new Inferno::Bullet(&purpleBullet_);
			born->Fire(m_onryouList[0]->GetPosition(), Inferno::Vec2<int>(-2, 0));
			purpleBullets.push_back(born);
			shootInterval.Restart();
			*/
		}
	}



}