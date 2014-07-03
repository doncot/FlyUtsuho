#include"ShooterElements.h"

#include<cmath>
#define PI       3.14159265358979323846

namespace Inferno
{
	void Bullet::Nway(const Inferno::Vec2<int>& bornPos, const int angle)
	{
		this->AMove(bornPos.x + m_idea->GetWidth(), bornPos.y + m_idea->GetHeight() / 2);

		m_vel.x = std::cosh(angle * PI / 180);
		m_vel.y = std::sinh(angle * PI / 180);
	}

}