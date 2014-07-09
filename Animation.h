/*
�A�j���[�V��������N���X
(�O��̃t���[������̌o�ߎ��Ԃ�Ⴆ�΂悢)
�h���N���X�œ���ȃA�j���[�V�����i�C�[�Y�C���Ƃ��j
�����Substance�n��
*/
#include<Timer.h>
#include<MyTypes.h>

namespace Inferno
{
//substance������A�j���̎�ނ�ݒ�
enum Animetype
{
	Transform,
};

class Animation
{
	//TODO�X�y���m�F
	enum TransitType
	{
		Liner,
		EasyIn,
	};
public:
	Animation();
	virtual ~Animation();


	//�f�B���C���ԁA�I�����ԁA�J�n�_�A���B�_�A���[�v�ݒ�A�i����/�񓯊�[�҂��҂��Ȃ���]�j
	//���B���W�̂�-1�Ō��ݍ��W
	void Start(const Millisec delay, const Millisec dur, const int s, const int e, const bool loopFlag=false);
	int GetVaule();
	bool HasEnded() const;

private:
	Timer m_timer;
	Millisec m_delay;
	Millisec m_dur;
	TransitType m_type;
	bool m_loop;
	int m_start;
	int m_end;
};



}