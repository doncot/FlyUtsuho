#include"ShooterElements.h"
#include"Inferno\Collision.h"

const double pi = std::acos(-1);

namespace Inferno
{
	void BulletOwner::ShootInternal(const Vec2<int> bornPos,const float degree, const int speed)
	{
		float x = std::cos(degree * pi / 180.0);
		x *= speed;
		float y = std::sin(degree * pi / 180.0);
		y *= speed;

		//�e����
		//���̃��\�[�X�}�l�[�W���[�̃V���O���g���͏����I�ɂ͂Ȃ�Ƃ����ׂ�
		auto newBullet = ResourceManager::CreateBulletInstance(L"redbullet");
		newBullet->Fire(bornPos, Vec2<int>(x, y));
		m_bullets.push_back(newBullet);
	}

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