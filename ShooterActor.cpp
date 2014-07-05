#include"ShooterElements.h"

namespace
{
	Inferno::Vec2<int> hitStartPoint;
}

namespace Inferno
{
	void ShooterActor::Hit()
	{
		if (m_curState == GEState::ge_neutral)
		{
			m_curState = GEState::ge_damage;
			m_timer.Start(1000);
			hitStartPoint = m_pos;
		}







	}

void ShooterActor::Update()
{
	if (m_curState == GEState::ge_damage)
	{
		//ダメージフェーズが終わったら
		if (!m_timer.HasFinished())
		{
			m_curState = GEState::ge_neutral;
		}
		
	}
}

}