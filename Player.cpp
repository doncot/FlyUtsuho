#include"ShooterElements.h"
#include"Animation.h"
#include"Inferno\Collision.h"
#include"GameCommon.h"
#include<cmath>
#include"ResourceManager.h"

const double pi = std::acos(-1);

namespace
{
	Inferno::Animation aTransX;
	Inferno::Animation aRotate;
}

namespace Inferno
{
	typedef Substance Base;

Player::~Player()
{
	for (auto i = m_bullets.begin(); i != m_bullets.end(); )
	{
		SAFE_DELETE((*i));
		i = m_bullets.erase(i);
	}
}

void Player::RMove(const int x, const int y)
{
	//斜め移動の場合、対処
	if (x == 0 || y == 0)
	{
		Base::RMove(x, y);
	}
	else
	{
		Base::RMove(x*0.71, y*0.71);
	}

	//範囲内にいるように調整
	Vec2<int> d;
	if (m_moveLimit.Height() != 0 && m_moveLimit.Width() != 0)
	{
		d = KeepSubInsideRect(this->GetRegion(), m_moveLimit);
		Base::AMove(d.x, d.y);
	}
}

void Player::RMove(const Vec2<int>& c)
{
	RMove(c.x, c.y);
}

void Player::SetMoveLimit(const Rect& rect)
{
	Rect temp = rect;
	temp.SetPosofULCorner(0, 0);
	m_moveLimit = temp;
}

void Player::Shoot(const float degree, const int speed)
{
	float x = std::cos(degree * pi / 180.0);
	x *= speed;
	float y = std::sin(degree * pi / 180.0);
	y *= speed;

	//弾生成
	auto newBullet = ResourceManager::CreateBulletInstance(L"redbullet");
	newBullet->Fire(m_pos, Vec2<int>(x,y));
	m_bullets.push_back(newBullet);
}

void Player::Hit()
{
	if (m_curState == PState::Neutral)
	{
		m_curState = PState::Damaged;
		/*
		aTransX.Start(0, 550, m_pos.x, m_pos.x - 200, Animation::TransitType::EaseOut);
		aRotate.Start(0, 600, 0, 720, Animation::TransitType::EaseOut);
		*/
		SetAttribute(GEAttribute::UserLock, true);
	}
}

void Player::Update()
{
	//自分へのダメージ
	if (m_curState == PState::Damaged)
	{
		//ダメージフェーズが終わったら
		if (aTransX.HasEnded())
		{
			m_curState = PState::Neutral;
			SetAttribute(GEAttribute::UserLock, false);
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

	//弾関連
	for (auto i = m_bullets.begin(); i != m_bullets.end();)
	{
		//殺して欲しい奴は殺す
		if ((*i)->CheckAttribute(GEAttribute::KillMe))
		{
			SAFE_DELETE(*i);
			i = m_bullets.erase(i);
			continue;
		}

		//移動
		(*i)->Update();
		

		i++;
	}
}

void Player::Draw(const Graphics& g) const
{
	//自分自身を描画
	Base::Draw(g);
	//弾の描画
	for (auto e : m_bullets)
	{
		e->Draw(g);
	}
}


}