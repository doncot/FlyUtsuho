#include<Timer.h>
#include<Windows.h>
#include<mmsystem.h> // timeGetTime—p
#pragma comment(lib, "winmm.lib") // VC++only

namespace Inferno
{
void Timer::Start()
{
	m_start = timeGetTime();
}

Millisec Timer::GetElapsed() const
{
	return timeGetTime() - m_start;
}

Millisec Timer::GetCurTime() const
{
	return timeGetTime();
}

void Timer::Restart()
{
	this->Start();
}

bool Timer::HasStarted() const
{
	return m_start != 0 ? true : false;
}

Stopwatch::Stopwatch() { m_targetTime = -1; }

Stopwatch::Stopwatch(const Millisec msec)
{
	this->Start(msec);
}

bool Stopwatch::HasFinished() const
{
	return m_timer.GetElapsed() > m_targetTime;
}

bool Stopwatch::HasStarted() const
{
	return m_timer.HasStarted();
}

void Stopwatch::Start(const Millisec msec)
{
	m_timer.Start();
	m_targetTime = msec;
}

void Stopwatch::Restart()
{
	m_timer.Restart();
}

double Stopwatch::DurationTimeRate() const
{
	return static_cast<double>(m_timer.GetElapsed()) / m_targetTime;
}

}