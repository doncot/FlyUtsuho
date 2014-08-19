#pragma once

#include<list>
#include<GameElements.h>
#include"MyTypes.h"
#include<Timer.h>
#include"Animation.h"
#include"ResourceManager.h"

namespace Inferno
{
//���ɂ���̂�
class Enemy;

//�p���p
class Damageable
{
public:
	//�v�f�ւ̃_���[�W��^����B�����������Łi�p����ŌĂяo���j
	//�Ԃ�l�F�_���[�W��Ԃ��I��������^��Ԃ�
	//blowVec�͐�����ԕ���
	bool EarnDamage(const Vec2<int>& blowVec = Vec2<int>())
	{
		//state���N���X�ɕ������Ă��̒��Ń|�����[�t�B�Y���Ƃ��Ď��s���鎖���ł���ȁA�����I��
		if ( !m_isInDamagedState )
		{
			m_isInDamagedState = true;
			/*
			aTransX.Start(0, 550, m_pos.x, m_pos.x - 200, Animation::TransitType::EaseOut);
			aRotate.Start(0, 600, 0, 720, Animation::TransitType::EaseOut);
			*/
			return false;
		}

		return true;
	}

private:
	//�_���[�W��Ԃ��ǂ����̃t���O
	bool m_isInDamagedState = false;;
};

class Bullet : public Substance
{
public:
	Bullet(const Inferno::Idea& idea) : Substance(idea) {}

	void Fire(const Inferno::Vec2<int>& bornPos, const int vel_x, const int vel_y)
	{
		this->AMove(bornPos.x + m_idea->GetWidth(), bornPos.y + m_idea->GetHeight() / 2);

		m_vel.x = vel_x;
		m_vel.y = vel_y;
	}

	void Fire(const Inferno::Vec2<int>& bornPos, const Inferno::Vec2<int>& vel)
	{
		this->Fire(bornPos, vel.x, vel.y);
	}
	
	//�A�j���[�V�����Ƃ��ړ��Ƃ�
	void Update();

private:
	typedef Inferno::Substance Base;
	Inferno::Vec2<int> m_vel;
};

//�e�����N���X�p�Ɋ֐����
class BulletOwner
{
public:
	//���g���Ǘ�����e���A�w��Ώۂɓ�������������
	bool CheckBulletHit(const Rect& hitbox) const;
	//�������������e��n���i�ǂݎ���p�j (���񂾂Ƃ��Ϗ�����z���ʂɍ��K�v�������)
	list<Bullet*> GetBulletList() const { return m_bullets; }
	void EraseGivenBullet(const Bullet& bullet);

protected:
	//�e���ꔭ���˂���i��ʂł�����Ăԁj
	//�p�x���w��
	void ShootInternal(const Vec2<int> bornPos, const float degree, const int speed);

	//���g���Ǘ�����e�̃��X�g
	std::list<Bullet*> m_bullets;
};

enum class PState
{
	Neutral,
	Damaged,
};
//enum class SEAnimeState;

class Player : public Substance, public BulletOwner, public Damageable
{
public:
	Player() : m_curState(PState::Neutral),m_moveLimit(0,0) {}
	~Player();

	//���ΕW�w��ړ��i�����I�ɂ͎��g�ňړ����Ǘ��������j
	void RMove(const int x, const int y);
	void RMove(const Vec2<int>& c);

	void SetMoveLimit(const Rect& rect);

	void Shoot(const float degree, const int speed);

	void Update();
	void Draw(const Graphics& g) const;

private:
	//�A�j���[�V�����p�̏��
	PState m_curState;
	Stopwatch m_timer;

	Rect m_moveLimit;

	//�萔
	const int HitBlowDistance = 20;
	Inferno::Vec2<int> hitStartPoint;
};

enum class EState
{
	Standby,
	Neutral,
	//Damage,
	Entry,
	Leave,
};

class Enemy : public Substance, public BulletOwner, public Damageable
{
public:
	Enemy();
	Enemy(const Idea& idea);

	void Entry();
	void Update();

private:
	Timer m_timer;
	EState m_curState = EState::Standby;
	Animation m_entryAnime[2];
	Animation m_anime[2];
	Animation m_exitAnime[2];

	//�萔
	const Millisec ShootInterval = 800;
};

}