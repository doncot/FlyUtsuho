#include"ShooterElements.h"
#include"Animation.h"
#include"Inferno\Collision.h"

namespace
{
	Inferno::Animation aTransX;
	Inferno::Animation aRotate;
}

namespace Inferno
{
void ShooterActor::RMove(const int x, const int y)
{
	Base::RMove(x, y);
	Vec2<int> d;
	if (m_moveLimit.Height() != 0 && m_moveLimit.Width() != 0)
	{
		d = KeepSubInsideRect(this->GetRegion(), m_moveLimit);
		Base::AMove(d.x, d.y);
	}
}

void ShooterActor::RMove(const Vec2<int>& c)
{
	RMove(c.x, c.y);
}

void ShooterActor::SetMoveLimit(const Rect& rect)
{
	Rect temp = rect;
	temp.SetPosofULCorner(0, 0);
	m_moveLimit = temp;
}

void ShooterActor::Hit()
{
	if (m_curState == SEState::se_neutral)
	{
		m_curState = SEState::se_damage;
		aTransX.Start(0, 600, m_pos.x, m_pos.x - 200, Animation::TransitType::TT_EaseOut);
		aRotate.KF_Clear();
		aRotate.KF_Set(0, 0, 400, 0, 400, Animation::TransitType::TT_Linear);
		aRotate.KF_Set(1, 400, 600, 600, 1080, Animation::TransitType::TT_EaseOut);
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
		m_pos.x = aTransX.GetValue();
		m_pos = KeepSubInsideRect(this->GetRegion(), m_moveLimit);

		m_angle = aRotate.GetValue();
	}
}

}