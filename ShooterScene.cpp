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
		//�d�����������甲��
		m_baseList.push_back(newBase);


		//�Y����id������Substance��EnemyBase���Z�b�g
		for (auto& e = m_subList.begin(); e != m_subList.end(); e++)
		{
			//id����v������
			if ((*e)->GetID() == id)
			{
				(*e)->SetIdea(newBase->m_idea);
			}
		}
	}

}