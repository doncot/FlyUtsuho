#include"ShooterElements.h"

namespace Inferno
{
	typedef Substance Base;

	void Bullet::Update()
	{
		Base::Update();

		//�ړ�
		this->RMove(m_vel);
	}
}