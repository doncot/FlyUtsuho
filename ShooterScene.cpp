#include"ShooterScene.h"

namespace Inferno
{
	ShooterScene::~ShooterScene()
	{
		for (auto i = m_subList.begin(); i != m_subList.end();)
		{
			SAFE_DELETE(*i);
			i = m_subList.erase(i);
			i++;
		}
	}

	void ShooterScene::CreateEnemy(const int id)
	{

	}

}