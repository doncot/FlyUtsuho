#include"ShooterElements.h"

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

		//アニメーション
		m_pos.x += -1; //左に下がる
		
	}
}

}