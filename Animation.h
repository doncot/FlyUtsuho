/*
�A�j���[�V��������N���X
(�O��̃t���[������̌o�ߎ��Ԃ�Ⴆ�΂悢)
�h���N���X�œ���ȃA�j���[�V�����i�C�[�Y�C���Ƃ��j
�����Substance�n��
*/
#pragma once

#include<Timer.h>
#include<MyTypes.h>
#include<vector>

namespace Inferno
{
	class ExternalTimer
	{
	public:
		ExternalTimer(const Timer& externalTimer) : m_timer(&externalTimer) {}
		void Start() { /*do nothing*/ }
	protected:
		Millisec GetElapsed() const { return m_timer->GetElapsed(); }
	private:
		const Timer* m_timer;
	};

	class LocalTimer
	{
	public:
		void Start() { m_timer.Start();	}
	protected:
		Millisec GetElapsed() const { return m_timer.GetElapsed(); }
	private:
		Timer m_timer;
	};



template<class TimerContainer, class AorR, class TransitType>
class Animation : public TimerContainer, public AorR, public TransitType
{
public:
	enum TransitType
	{
		Unknown,
		Linear,
		EaseIn,
		EaseOut,
		EaseInOut,
	};
	enum class MoveType
	{
		Unknown,
		Relative,
		Absolute
	};

	struct KeyFrameSet
	{
		KeyFrameSet() { this->Clear(); }
		int key;
		Millisec delay;
		Millisec dur;
		TransitType type;
		MoveType movetype;
		bool loop;
		int start;
		int end;

		void Clear()
		{
			key = 0;
			delay = 0;
			dur = 0;
			type = TransitType::Unknown;
			movetype = MoveType::Unknown;
			loop = false;
			start = 0;
			end = 0;
		}
	};

public:
	Animation();
	virtual ~Animation();

	//�f�B���C���ԁA�I�����ԁA�J�n�_�A���B�_�A���[�v�ݒ�A�i����/�񓯊�[�҂��҂��Ȃ���]�j
	//���B���W�̂�-1�Ō��ݍ��W
	void Set(const Millisec delay, const float approachSpeed, const int d, const TransitType type, const bool loopFlag = false);
	void Set(const Millisec delay, const Millisec dur, const int s, const int e, const TransitType type, const bool loopFlag = false);
	void KF_Set(const int key, const Millisec delay, const Millisec approachSpeed,
		const int d, const TransitType type = Linear, const bool loopFlag = false);
	void KF_Set(const int key,const Millisec delay, const Millisec dur,
		const int s, const int e, const TransitType type = Linear, const bool loopFlag = false);
	//�ݒ�Ɠ����ɃX�^�[�g�i��Ƀe�X�g�p�j
	//void LocalStart(const Millisec delay, const Millisec dur, const int s, const int e, const TransitType type, const bool loopFlag = false);

	//���Έړ��̏ꍇ�A���݂̍��W���K�v
	int GetValueByCurrent(const int curVal);
	int GetValue();
	bool HasEnded() const;
	//�L�[�t���[����ݒ肵���ꍇ�A�Ăюg���O�Ƀ��Z�b�g
	void KF_Clear();

	static const TransitType InterpretTransitType(const std::wstring str);

private:
	std::vector<KeyFrameSet> m_kfset;
	int m_curKey;
	int m_endKey;
	Millisec m_endTime;
};



}