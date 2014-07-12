#include"Animation.h"
#include<sstream>
#include<vector>
#include<cmath>
#include<cassert>

namespace
{
	double Linear(double change, double base, double duration, double time)
	{
		return change * ( time / duration ) + base;
	}

	//From: Robert Penner's Programming Macromedia Flash MX by Robert Penner (BSD license!!)
	double EaseInQuad(double change, double base, double duration, double time) {
		time /= duration;
		return change * time * time + base;
	}

	double EaseOutQuad(double change, double base, double duration, double time) {
		time /= duration;
		return -change * time *(time - 2) + base;
	}

	double EaseInOutQuad(double change, double base, double duration, double time) {
		time /= duration / 2;
		if (time < 1) return change / 2 * time*time + base;
		return -change / 2 * ((--time)*(time - 2) - 1) + base;
	}
}

namespace Inferno
{
Animation::Animation()
	:m_endKey(0), m_endTime(0)
{
}

Animation::~Animation()
{
}

void Animation::Start(const Millisec delay, const Millisec dur, const int s, const int e,
		const Animation::TransitType type, const bool loopFlag)
{
	KF_Clear();
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
	for (int i = 0; i <= m_endKey; i++)
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
	assert(0);
	return 0;
}

bool Animation::HasEnded() const
{
	return m_timer.GetElapsed() > m_endTime;
}

void Animation::KF_Clear()
{
	for (int i = 0; i < m_endKey; i++)
	{
		m_kfset[i].Clear();
	}
	m_curKey = 0;
	m_endKey = 0;
	m_endTime = 0;
}

}