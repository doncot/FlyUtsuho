/*
Score.h
�X�R�A�v���p�N���X
���X�R�A���ω����Ă�����Ԋu���Ƃɑ������J��Ԃ�
*/
#pragma once

#include<string>
#include<sstream>
#include<Timer.h>
#include<cmath>

namespace Inferno
{
template<typename T>
class Score
{
public:
	Score() : m_score(0),m_displayScore(0), m_permitNegative(false) {}

	//�A�j���[�V�����p�̍X�V�֐�
	void Update()
	{
		if (m_intervalTimer.HasFinished())
		{
			m_displayScore = m_score;
		}
		else if (m_intervalTimer.HasStarted())
		{
			m_displayScore = static_cast<int>( (m_score - m_previousScore) * m_intervalTimer.DurationTimeRate()) + m_previousScore;
		}

		//�ݒ�ɂ���ă}�C�i�X�܂ōs������s���Ȃ�������
		if ( !m_permitNegative && (m_displayScore < 0) )
		{
			m_displayScore = 0;
			m_score = 0;
		}
	}

	T GetScore() const { return m_score; }
	//�\���p�̃X�R�A
	std::wstring DisplayScore() const
	{
		std::wstringstream ss;
		ss << m_displayScore;
		return ss.str();
	}

	//void operator=(const T score){ m_score = score; }
	void operator+=(const T score)
	{
		m_intervalTimer.Start(Interval);
		m_previousScore = m_score;
		m_score += score; 
	}
	void operator-=(const T score) 
	{
		m_intervalTimer.Start(Interval);
		m_previousScore = m_score;
		m_score -= score; 
	}
	//void operator*=(const T score) { m_score *= score; }

private:
	const Millisec Interval = 800; //�X�R�A���ŏI�I�Ȓl�܂Ő��ڂ��鎞��
	Inferno::Stopwatch m_intervalTimer;

	T m_score;
	T m_previousScore;
	//�h�������[���A�j���̂��߂̕\���p����
	T m_displayScore;

	//���̐����������ۂ��B�f�t�H���g�Ŗ���
	bool m_permitNegative;
};




}