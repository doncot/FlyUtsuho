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

Player::~Player()
{
	SAFE_DELETE(m_fireball);
}

void Player::LoadResource()
{
	//自分のリソース
	//将来的にはテクスチャはリソースマネージャーが持つため、イデアはここでロードしてよい
	m_utsuhoTex.LoadImageFile(*m_graphics, TEXT("Sprites\\utsuho.png"));
	m_utsuho_.SetTexture(m_utsuhoTex);
	this->SetIdea(&m_utsuho_);
	this->AMove(200, 200);
	this->SetMoveLimit(Inferno::Rect(700, 600));

	//弾（将来的にはリソースマネージャーから入手する）
	m_fireballTex.LoadImageFile(*m_graphics, TEXT("Sprites\\fireball.png"));
	m_fireball_.SetTexture(m_fireballTex);
	m_fireball = nullptr;
}

void Player::RMove(const int x, const int y)
{
	Base::RMove(x, y);
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

void Player::AcceptPlayerControl()
{
	if (m_input->IsKeyDown(VK_UP))
	{
		this->RMove(0, -MoveVel);
	}
	if (m_input->IsKeyDown(VK_DOWN))
	{
		this->RMove(0, MoveVel);
	}
	if (m_input->IsKeyDown(VK_LEFT))
	{
		this->RMove(-MoveVel, 0);
	}
	if (m_input->IsKeyDown(VK_RIGHT))
	{
		this->RMove(MoveVel, 0);
	}
	if (m_input->IsKeyPressed('Z'))
	{
		if (m_fireball == nullptr)
		{
			m_fireball = new Inferno::Bullet(&m_fireball_);
			m_fireball->Fire(this->GetPosition(), 5, 0);

			//SE
			//fire.Stop();
			//fire.Play();
		}
	}
}

void Player::SetMoveLimit(const Rect& rect)
{
	Rect temp = rect;
	temp.SetPosofULCorner(0, 0);
	m_moveLimit = temp;
}

void Player::SetInput(const Input& input)
{
	m_input = &input;
}

void Player::Hit()
{
	if (m_curState == SEState::se_neutral)
	{
		m_curState = SEState::se_damage;
		aTransX.Start(0, 550, m_pos.x, m_pos.x - 200, Animation::TransitType::TT_EaseOut);
		aRotate.Start(0, 600, 0, 720, Animation::TransitType::TT_EaseOut);
		SetAttribute(GEAttribute::ge_userLock, true);
	}
}

void Player::Update()
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

	//弾
	if (m_fireball) m_fireball->Update();
}

}