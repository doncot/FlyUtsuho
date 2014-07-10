#include"ShooterElements.h"
#include"Animation.h"

namespace
{
	Inferno::Animation aTransX;
	Inferno::Animation aRotate;
}

namespace Inferno
{
void ShooterActor::Hit()
{
	if (m_curState == SEState::se_neutral)
	{
		m_curState = SEState::se_damage;
		aTransX.Start(0, 600, m_pos.x, m_pos.x - 200, Animation::TransitType::TT_Linear);
		aRotate.KF_Clear();
		aRotate.KF_Set(0, 0, 400, 0, 400, Animation::TransitType::TT_Linear);
		aRotate.KF_Set(1, 400, 300, 600, 720, Animation::TransitType::TT_EaseOut);
		SetAttribute(GEAttribute::ge_userLock, true);
	}
}

void ShooterActor::Update()
{
	if (m_curState == SEState::se_damage)
	{
		//ダメージフェーズが終わったら
		if (aTransX.HasEnded() && aRotate.HasEnded())
		{
			m_curState = SEState::se_neutral;
			SetAttribute(GEAttribute::ge_userLock, false);
		}

		//アニメーション
		m_pos.x = aTransX.GetValue(); //左に下がる
		m_angle = aRotate.GetValue();
	}
}

}