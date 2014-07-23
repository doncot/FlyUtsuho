#pragma once
#include"Inferno\GameElements.h"
#include<list>

namespace Inferno
{
class ShooterScene
{
public:
	~ShooterScene();

	void CreateEnemy(const int id);
	void DeleteEnemy(const int id);


private:
	std::list<Substance*> m_subList;
};


}