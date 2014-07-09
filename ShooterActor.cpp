#include"ShooterElements.h"
#include"Animation.h"

namespace
{
	Inferno::Animation aRotate;
}

namespace Inferno
{
void ShooterActor::Hit()
{
	if (m_curState == SEState::se_neutral)
	{
		m_curState = SEState::se_damage;
		aRotate.Start(0,600,70,720);
		SetAttribute(GEAttribute::ge_userLock, true);
	}
}

void ShooterActor::Update()
{
	if (m_curState == SEState::se_damage)
	{
		//�_���[�W�t�F�[�Y���I�������
		if (aRotate.HasEnded())
		{
			m_curState = SEState::se_neutral;
			SetAttribute(GEAttribute::ge_userLock, false);
		}

		//�A�j���[�V����
		m_pos.x += -8; //���ɉ�����
		m_angle = aRotate.GetVaule();
	}
}

}