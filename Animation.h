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
	enum Transittype
	{
		Liner,
		EasyIn,
	};
public:
	Animation();
	virtual ~Animation();


	//�f�B���C���ԁA�I�����ԁA�J�n���W�A���B���W�A���[�v�ݒ�A�i����/�񓯊�[�҂��҂��Ȃ���]�j
	//���B���W�̂�-1�Ō��ݍ��W
	void SetTransform(const Millisec delay, const Millisec dur, const Vec2<int> s, const Vec2<int> e, const bool loopFlag=false);

	//�����x��ω�������
	////�f�B���C���ԁA�I�����ԁA�J�n�����x�A���B�����x�A���[�v�ݒ�A�i����/�񓯊�[�҂��҂��Ȃ���]�j
	void SetAlpha(Millisec delay, Millisec dur, float s, float e);

	Vec2<int> DoTransform(const Millisec frameTime) const;
	float DoAlpha(const Millisec frameTime);

	//substance�ɃI�[�o�[���C�h���Ă��炤
	//��ŏ����ɂ���
	virtual void Update(){};
	
private:
	Timer m_timer;
	Millisec m_delay;
	Millisec m_dur;
	Animetype m_type;
	//transform
	Vec2<int> m_startCor;
	Vec2<int> m_endCor;
	//rotate
	//scale
	//alpha
	float m_startAlpha;
	float m_endAlpha;
	bool m_loop;
};



}