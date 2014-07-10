#include"Animation.h"
#include<sstream>
#include<vector>
#include<cmath>
#include<cassert>

namespace
{
	double EaseInCube(int x)
	{
		double result = 0;
		bool negative = false;
		if (x < 0)
		{
			x *= -1;
			negative = true;
		}
		else if (x == 0) return 0;

		double tx = x / std::sqrt(x); //1に正規化
		result = tx*tx*tx;

		if (!negative) return result;
		else return result *= -1;
	}

	double EaseOutCube(int x)
	{
		double result = 0;
		bool negative = false;
		if (x < 0)
		{
			x *= -1;
			negative = true;
		}
		else if (x == 0) return 0;

		double tx = x / std::sqrt(x); //1に正規化
		result = 1.0 - (1.0 - tx)*(1.0 - tx)*(1.0 - tx);

		if (!negative) return result;
		else return result *= -1;
	}

	double Linear(double change, double base, double duration, double time)
	{
		return change * ( time / duration ) + base;
	}

	//From: Robert Penner's Programming Macromedia Flash MX by Robert Penner (BSD license!!)
	double EaseInQuad(double change, double base, double duration, double time) {
		time /= duration;
		return change * time * time + base;
	};

	double EaseOutQuad(double change, double base, double duration, double time) {
		time /= duration;
		return change * sqrt(sqrt(time)) + base;
	};
}

namespace Inferno
{
Animation::Animation()
	:m_endKey(0)
{
}

Animation::~Animation()
{
}

void Animation::Start(const Millisec delay, const Millisec dur, const int s, const int e,
		const Animation::TransitType type, const bool loopFlag)
{
	KF_Set(0, delay, dur, s, e, type, loopFlag);
}

void Animation::KF_Set(const int key,const Millisec delay, const Millisec dur,
	const int s, const int e, const Animation::TransitType type, const bool loopFlag)
{
	if (key >= m_kfset.size()) 
		m_kfset.resize(key+1);
	m_kfset[key].delay = delay;
	m_kfset[key].dur = dur;
	m_kfset[key].start = s;
	m_kfset[key].end = e;
	m_kfset[key].type = type;
	m_kfset[key].loop = loopFlag;
	m_timer.Start();
	if (key > m_endKey)
	{
		m_endKey = key;	
	}
	//最終経過を計算
	for (int i = 0; i < m_endKey + 1; i++)
	{
		m_endTime += m_kfset[i].delay + m_kfset[i].dur;
	}
}

int Animation::GetValue()
{
	Millisec etime = m_timer.GetElapsed();
	//時満たず
	if (etime < m_kfset[m_curKey].delay)
	{
		return m_kfset[m_curKey].start;
	}
	//現在のキーが終わったら
	if (etime > m_kfset[m_curKey].delay + m_kfset[m_curKey].dur)
	{
		//今が最後のキーでなければ、次のキーへインデックスを進める
		if (m_curKey < m_endKey) m_curKey++;
		//いずれにせよ今回はこのキーの終わりを返す
		return m_kfset[m_curKey].end;
	}

	switch (m_kfset[m_curKey].type)
	{
	case TT_Linear:
		return Linear(m_kfset[m_curKey].end - m_kfset[m_curKey].start,
			m_kfset[m_curKey].start, m_kfset[m_curKey].dur,
			etime - m_kfset[m_curKey].delay);
	case TT_EaseIn:
		return EaseInQuad(m_kfset[m_curKey].end - m_kfset[m_curKey].start,
			m_kfset[m_curKey].start, m_kfset[m_curKey].dur,
			etime - m_kfset[m_curKey].delay);
	case TT_EaseOut:
		return EaseOutQuad(m_kfset[m_curKey].end - m_kfset[m_curKey].start,
			m_kfset[m_curKey].start, m_kfset[m_curKey].dur,
			etime - m_kfset[m_curKey].delay);
	default:
		assert(0);
	}

	//ここに来ることはない
	return 0;
}

bool Animation::HasEnded() const
{
	return m_timer.GetElapsed() > m_endTime;
}


}