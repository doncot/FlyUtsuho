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

	double EaseInQuart(double change, double base, double duration, double time)
	{
		return -change * (std::pow(time / duration - 1, 4) - 1) + base;
	}
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

void Animation::SetTransform(const Millisec delay,const Millisec dur,const Vec2<int> s,const Vec2<int> e,const bool loopFlag)
{
	Vec2<int> temp=e;
	m_delay = delay;
	m_dur = dur;
	m_startCor = s;
	if (e.x == -1)
	{
		temp.x = s.x;
	}
	if (e.y == -1)
	{
		temp.y = s.y;
	}
	m_endCor = temp;
	m_loop = loopFlag;
	m_timer.Start();
}

void Animation::SetAlpha(Millisec delay, Millisec dur, float s, float e)
{
	m_delay = delay;
	m_dur = dur;
	m_startAlpha = s;
	m_endAlpha = e;

	m_timer.Start();
}

Vec2<int> Animation::DoTransform(Millisec) const
{
	Vec2<int> inner;
	Millisec etime = m_timer.GetElapsed();
	//Žž–ž‚½‚¸
	if (etime < m_delay)
	{
		return m_startCor;
	}
	//I‚í‚Á‚½‚ç‹A‚ê
	if (etime > m_delay + m_dur)
	{
		return m_endCor;
	}
	//double d = (etime - m_delay) / static_cast<double>(m_dur);
	inner.x = EaseInQuart( m_endCor.x - m_startCor.x, m_startCor.x, m_dur, etime - m_delay);
	//inner.x = EaseInCube(m_endCor.x - m_startCor.x) * d;
	inner.y = EaseInQuad( m_endCor.y - m_startCor.y, m_startCor.y, m_dur, etime - m_delay); 

	return inner;
}

float Animation::DoAlpha(Millisec)
{
	float inner;
	Millisec etime = m_timer.GetElapsed();
	//Žž–ž‚½‚¸
	if (etime < m_delay)
	{
		return m_startAlpha;
	}
	//I‚í‚Á‚½‚ç‹A‚ê
	if (etime > m_delay + m_dur)
	{
		return m_endAlpha;
	}

	inner = (m_endAlpha - m_startAlpha) * (etime - m_delay) / m_dur;

	return m_startAlpha + inner;
}

}