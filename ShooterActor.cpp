#include"ShooterElements.h"
#include"Animation.h"
#include"Inferno\Collision.h"
#include"GameCommon.h"

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
		aTransX.Start(0, 550, m_pos.x, m_pos.x - 200, Animation::TransitType::TT_EaseOut);
		aRotate.Start(0, 600, 0, 720, Animation::TransitType::TT_EaseOut);
		SetAttribute(GEAttribute::ge_userLock, true);
	}
}

void ShooterActor::Update()
{
	if (m_curState == SEState::se_damage)
	{
		//ダメージフェーズが終わったら
		if (aTransX.HasEnded())
		{
			m_curState = SEState::se_neutral;
			SetAttribute(GEAttribute::ge_userLock, false);
			m_angle = 0; //ここでリセットしないと次も値が溜まったままになる
			SetAlpha(0xff);
		}
		else
		{
			//アニメーション
			//移動
			m_pos.x = aTransX.GetValue();
			//簡易版
			//double dx = (120 - m_pos.x) / 30.0;
			//m_pos.x += dx;
			m_pos = KeepSubInsideRect(this->GetRegion(), m_moveLimit);
			//回転
			m_angle = aRotate.GetValue();
			//double dAngle = (720 - m_angle) / 10.0;
			//m_angle += dAngle;

			//点滅
			static int count = 0;
			SetAlpha(Inferno::Blink(count++, 0xff, 25));
		}
	}
}

}