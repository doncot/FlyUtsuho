#include"ShooterScene.h"

namespace Inferno
{
	ShooterScene::~ShooterScene()
	{
		for (auto i = m_subList.begin(); i != m_subList.end();)
		{
			SAFE_DELETE(*i);
			i = m_subList.erase(i);
		}

		for (auto i = m_baseList.begin(); i != m_baseList.end();)
		{
			SAFE_DELETE(*i);
			i = m_baseList.erase(i);
		}
	}

	void ShooterScene::CreateEnemy(const int id)
	{
		auto insert = new Substance();
		insert->SetID(id);
		m_subList.push_back(insert);
	}

	void ShooterScene::SetTexture(const int id, const Texture& tex)
	{
		auto newBase = new EnemyBase();
		newBase->m_tex = tex;
		newBase->m_idea.SetTexture(newBase->m_tex);
		//重複があったら抜く
		m_baseList.push_back(newBase);


		//該当のidを持つSubstanceにEnemyBaseをセット
		for (auto& e = m_subList.begin(); e != m_subList.end(); e++)
		{
			//idが一致したら
			if ((*e)->GetID() == id)
			{
				(*e)->SetIdea(newBase->m_idea);
			}
		}
	}

}