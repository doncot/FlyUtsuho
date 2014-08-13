#include"ShooterElements.h"
#include"Inferno\Collision.h"

const double pi = std::acos(-1);

namespace Inferno
{
	void BulletOwnSubstance::Shoot(const float degree, const int speed)
	{
		float x = std::cos(degree * pi / 180.0);
		x *= speed;
		float y = std::sin(degree * pi / 180.0);
		y *= speed;

		//弾生成
		//このリソースマネージャーのシングルトンは将来的にはなんとかすべき
		auto newBullet = ResourceManager::CreateBulletInstance(L"redbullet");
		newBullet->Fire(m_pos, Vec2<int>(x, y));
		m_bullets.push_back(newBullet);
	}

	bool BulletOwnSubstance::CheckBulletHit(const Rect& hitbox) const
	{
		//どれか1つの弾に当たったら、そのフレームでは処理を終える
		for (auto bullet : m_bullets)
		{
			if (IsRect1HittingRect2(bullet->GetHitBox(), hitbox))
			{
				return true;
			}
		}
		return false;
	}

	void BulletOwnSubstance::EraseGivenBullet(const Bullet& bullet)
	{
		for (auto i = m_bullets.begin(); i != m_bullets.end();)
		{
			//指定された弾を処分する
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