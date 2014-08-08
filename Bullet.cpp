#include"ShooterElements.h"

namespace Inferno
{
	typedef Substance Base;

	void Bullet::Update()
	{
		Base::Update();

		//ˆÚ“®
		this->RMove(m_vel);
	}
}