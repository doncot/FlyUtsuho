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

	//LoadImage�̓}�N����������Ă���
	void Loadimage(const int id, const string& filename);

private:
	std::list<Substance*> m_subList;
};


}