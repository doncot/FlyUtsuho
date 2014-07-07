#pragma once

#include<Game.h>
#include<Texture.h>
#include<GameElements.h>
#include"ShooterElements.h"
#include<list>
#include"Score.h"

class SVShooter final : public Game
{
public:
	SVShooter();
	~SVShooter();

	bool Initialize();
	bool GameLoop();

	enum SceneName
	{
		sn_main,
		sn_title,
		sn_result,
	};
private:
	typedef Game Base;

	SceneName m_scene;

	Inferno::Rect m_screenRect;

	Inferno::Instant m_titleImage;

	Inferno::Texture m_utsuhoTex;
	Inferno::Idea m_utsuho_;
	Inferno::ShooterActor* m_utsuho;

	Inferno::Texture m_fireballTex;
	Inferno::Idea m_fireball_;
	Inferno::Bullet* m_fireball;

	Inferno::Texture m_onryouTex;
	Inferno::Idea m_onryou_;
	std::vector<Inferno::Substance*> m_onryouList;

	Inferno::Score<int> m_score;
};