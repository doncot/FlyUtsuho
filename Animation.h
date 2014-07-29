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
class Animation
{
public:
	//TODO�X�y���m�F
	enum TransitType
	{
		Unknown,
		Linear,
		EaseIn,
		EaseOut
	};

	Animation();
	virtual ~Animation();


	//�f�B���C���ԁA�I�����ԁA�J�n�_�A���B�_�A���[�v�ݒ�A�i����/�񓯊�[�҂��҂��Ȃ���]�j
	//���B���W�̂�-1�Ō��ݍ��W
	void Set(const Millisec delay, const Millisec dur, const int s, const int e, const TransitType type, const bool loopFlag = false);
	void KF_Set(const int key,const Millisec delay, const Millisec dur,
		const int s, const int e, const TransitType type = Linear, const bool loopFlag = false);
	//�ݒ�Ɠ����ɃX�^�[�g�i��Ƀe�X�g�p�j
	void Start(const Millisec delay, const Millisec dur, const int s, const int e, const TransitType type, const bool loopFlag = false);
	//�o�^�ς݂̃X�^�[�g
	void Start();
	int GetValue();
	bool HasEnded() const;
	//�L�[�t���[����ݒ肵���ꍇ�A�Ăюg���O�Ƀ��Z�b�g
	void KF_Clear();

	const TransitType InterpretTransitType(const std::wstring str) const;

private:
	struct KeyFrameSet
	{
		KeyFrameSet() :key(0), delay(0), dur(0), loop(false), start(0), end(0){}
		int key;
		Millisec delay;
		Millisec dur;
		TransitType type;
		bool loop;
		int start;
		int end;

		void Clear()
		{
			key = 0;
			delay = 0;
			dur = 0;
			type = Linear;
			loop = false;
			start = 0;
			end = 0;
		}
	};

	Timer m_timer;
	std::vector<KeyFrameSet> m_kfset;
	int m_curKey;
	int m_endKey;
	Millisec m_endTime;
};



}