#include"Animation.h"
#include<sstream>
#include<vector>
#include<cmath>

namespace
{
	int Linear(const int x)
	{
		return x;
	}
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

		double tx = x / std::sqrt(x); //1‚É³‹K‰»
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

		double tx = x / std::sqrt(x); //1‚É³‹K‰»
		result = 1.0 - (1.0 - tx)*(1.0 - tx)*(1.0 - tx);

		if (!negative) return result;
		else return result *= -1;
	}

	//From: Robert Penner's Programming Macromedia Flash MX by Robert Penner (BSD license!!)
	double EaseInQuad(double change, double base, double duration, double time) {
		time /= duration;
		return change * time * time + base;
	};
}

namespace Inferno
{
Animation::Animation()
	:m_dur(0), m_delay(0)
{
}

Animation::~Animation()
{
}

void Animation::Start(const Millisec delay, const Millisec dur, const int s, const int e,
		const bool loopFlag = false)
{
	m_delay = delay;
	m_dur = dur;
	m_start = s;
	m_end = e;
	m_loop = loopFlag;
	m_timer.Start();
}

int Animation::GetVaule()
{
	Millisec etime = m_timer.GetElapsed();
	//Žž–ž‚½‚¸
	if (etime < m_delay)
	{
		return m_start;
	}
	//I‚í‚Á‚½‚ç‹A‚ê
	if (etime > m_delay + m_dur)
	{
		return m_end;
	}

	return EaseInQuad(m_end - m_start, m_start, m_dur, etime - m_delay);

}

bool Animation::HasEnded() const
{
	return m_timer.GetElapsed() > m_delay + m_dur;
}


}