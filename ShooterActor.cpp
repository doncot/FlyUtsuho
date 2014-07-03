#include"ShooterElements.h"

namespace Inferno
{

void ShooterActor::Hit()
{
	if (m_curState == GEState::ge_neutral)
	{
		m_curState = GEState::ge_damage;
		m_timer.Start(1000);
	}
}

void ShooterActor::Update() //これは派生クラスで定義するべきだな……
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