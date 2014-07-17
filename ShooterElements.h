#pragma once

#include<GameElements.h>
#include"MyTypes.h"
#include<Timer.h>
#include"Inferno\Input.h"
#include"Inferno\Graphics.h"

namespace Inferno
{
class Bullet : public Substance
{
public:
	Bullet(Inferno::Idea* idea) : Substance(idea) {}

	void Fire(const Inferno::Vec2<int>& bornPos, const int x, const int y)
	{
		this->AMove(bornPos.x + m_idea->GetWidth(), bornPos.y + m_idea->GetHeight() / 2);

		m_vel.x = x;
		m_vel.y = y;
	}

	void Fire(const Inferno::Vec2<int>& bornPos, const Inferno::Vec2<int>& vel)
	{
		this->Fire(bornPos, vel.x, vel.y);
	}

	//Nway�e�����
	void Nway(const Inferno::Vec2<int>& bornPos, const int angle);

	//�A�j���[�V�����Ƃ��ړ��Ƃ�
	void Update()
	{
		this->RMove(m_vel);
	}

private:
	typedef Inferno::Substance Base;
	Inferno::Vec2<int> m_vel;

};

enum class SEState
{
	se_neutral,
	se_damage
};
//enum class SEAnimeState;

class Player : public Substance
{
public:
	typedef Substance Base;

	Player(const Graphics& g)
		: m_curState(SEState::se_neutral),m_moveLimit(0,0),m_graphics(&g),
		m_input(nullptr) {}

	Player(const Graphics& g, const Input& input)
		: m_curState(SEState::se_neutral), m_moveLimit(0, 0), m_graphics(&g),
		m_input(&input) {}

	//�����I�ɂ́AInitialize(const ResourseManager& rm)�݂����Ȋ�����
	void LoadResource();

	//�v���C���[�̓��͂��󂯕t����
	void AcceptPlayerControl();

	//���ΕW�w��ړ�
	void RMove(const int x, const int y);
	void RMove(const Vec2<int>& c);

	void SetMoveLimit(const Rect& rect);
	void SetInput(const Input& input);

	void Hit();
	void Update();

private:
	const Input* m_input;
	const Graphics* m_graphics;

	//���\�[�X�i�����I�ɂ�ResourseManager�I�ȂƂ��납�玝���Ă���j
	Inferno::Texture m_utsuhoTex;
	Inferno::Idea m_utsuho_;

	Inferno::Texture m_fireballTex;
	Inferno::Idea m_fireball_;
	Inferno::Bullet* m_fireball;

	//�A�j���[�V�����p�̏��
	SEState m_curState;
	Stopwatch m_timer;
	//������͈�
	Rect m_moveLimit;

	//�萔
	const int MoveVel = 6;
	const int HitBlowDistance = 20;
	Inferno::Vec2<int> hitStartPoint;
};

}