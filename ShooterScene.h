#pragma once
#include"Inferno\GameElements.h"
#include<list>

namespace Inferno
{

class ShooterScene
{
private:
	struct EnemyBase
	{
		Inferno::Idea m_idea;
		Inferno::Texture m_tex;
	};

public:
	~ShooterScene();

	void CreateEnemy(const int id);
	void DeleteEnemy(const int id);

	void SetTexture(const int id, const Texture& tex);


private:
	std::list<Substance*> m_subList;
	std::list<EnemyBase*> m_baseList;
};


}