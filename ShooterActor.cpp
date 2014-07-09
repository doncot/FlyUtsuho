#include"ShooterElements.h"

namespace Inferno
{
void ShooterActor::Hit()
{
	if (m_curState == SEState::se_neutral)
	{
		m_curState = SEState::se_damage;
		m_timer.Start(500);
		hitStartPoint = m_pos;
		SetAttribute(GEAttribute::ge_userLock, true);
	}
}

void ShooterActor::Update()
{
	if (m_curState == SEState::se_damage)
	{
		//�_���[�W�t�F�[�Y���I�������
		if (m_timer.HasFinished())
		{
			m_curState = SEState::se_neutral;
			SetAttribute(GEAttribute::ge_userLock, false);
		}

		//�A�j���[�V����
		m_pos.x += -8; //���ɉ�����
		m_angle += 30; //��]
	}
}

}