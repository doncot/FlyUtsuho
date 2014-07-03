/*
Score.h
スコア計測用クラス
※スコアが変化してから一定間隔ごとに増減を繰り返す
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

	//アニメーション用の更新関数
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

		//設定によってマイナスまで行ったり行かなかったり
		if ( !m_permitNegative && (m_displayScore < 0) )
		{
			m_displayScore = 0;
			m_score = 0;
		}
	}

	T GetScore() const { return m_score; }
	//表示用のスコア
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
	const Millisec Interval = 800; //スコアが最終的な値まで推移する時間
	Inferno::Stopwatch m_intervalTimer;

	T m_score;
	T m_previousScore;
	//ドラムロールアニメのための表示用部分
	T m_displayScore;

	//負の数を許すか否か。デフォルトで無効
	bool m_permitNegative;
};




}