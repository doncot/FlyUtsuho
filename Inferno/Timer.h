#pragma once

typedef unsigned long int Millisec;
const int FixFPS = 60;

namespace Inferno
{
	class Timer
	{
	public:
		Timer() :m_start(0){};
		~Timer(){};

		void Start();
		Millisec Timer::GetElapsed() const;
		void Restart();

		bool HasStarted() const;

	private:
		Millisec m_start;
	};

	//指定時間経過しているかどうかを判定するクラス
	class Stopwatch
	{
	public:
		Stopwatch();
		Stopwatch(const Millisec msec);
		bool HasFinished() const;
		bool HasStarted() const;
		void Start(const Millisec msec);
		//同じ設定時間で再起動
		void Restart();
		double DurationTimeRate() const;

	private:
		Timer m_timer;
		Millisec m_targetTime;
	};
}