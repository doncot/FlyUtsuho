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

	//�w�莞�Ԍo�߂��Ă��邩�ǂ����𔻒肷��N���X
	class Stopwatch
	{
	public:
		Stopwatch();
		Stopwatch(const Millisec msec);
		bool HasFinished() const;
		bool HasStarted() const;
		void Start(const Millisec msec);
		//�����ݒ莞�ԂōċN��
		void Restart();
		double DurationTimeRate() const;

	private:
		Timer m_timer;
		Millisec m_targetTime;
	};
}