#include"ShooterElements.h"
#include"Animation.h"

namespace
{
	Inferno::Animation aRotate;
}

namespace Inferno
{
void ShooterActor::Hit()
{
	if (m_curState == SEState::se_neutral)
	{
		m_curState = SEState::se_damage;
		aRotate.Start(0,600,70,720);
		SetAttribute(GEAttribute::ge_userLock, true);
	}
}

void ShooterActor::Update()
{
	if (m_curState == SEState::se_damage)
	{
		//ダメージフェーズが終わったら
		if (aRotate.HasEnded())
		{
			m_curState = SEState::se_neutral;
			SetAttribute(GEAttribute::ge_userLock, false);
		}

		//アニメーション
		m_pos.x += -8; //左に下がる
		m_angle = aRotate.GetVaule();
	}
}

}