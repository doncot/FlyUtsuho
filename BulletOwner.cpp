#include"ShooterElements.h"
#include"Inferno\Collision.h"

namespace Inferno
{
	bool BulletOwner::CheckBulletHit(const Rect& hitbox) const
	{
		//�ǂꂩ1�̒e�ɓ���������A���̃t���[���ł͏������I����
		for (auto bullet : m_bullets)
		{
			if (IsRect1HittingRect2(bullet->GetHitBox(), hitbox))
			{
				return true;
			}
		}
		return false;
	}

	void BulletOwner::EraseGivenBullet(const Bullet& bullet)
	{
		for (auto i = m_bullets.begin(); i != m_bullets.end();)
		{
			//�w�肳�ꂽ�e����������
			if (*i == &bullet)
			{
				SAFE_DELETE(*i);
				i = m_bullets.erase(i);
				return;
			}
			i++;
		}
	}

}