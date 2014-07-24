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

	//LoadImage‚Íƒ}ƒNƒ‰˜õ‚³‚ê‚Ä‚¢‚é
	void Loadimage(const int id, const string& filename);

private:
	std::list<Substance*> m_subList;
};


}