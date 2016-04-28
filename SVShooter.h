#pragma once

#include<Game.h>
#include<Texture.h>
#include<GameElements.h>
#include"ShooterElements.h"
#include<list>
#include"Score.h"

class SVShooter final : public Inferno::Game
{
public:
	SVShooter();
	~SVShooter();

	void Startup();
	bool GameLoop() override;

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
	Inferno::Player* m_utsuho;

	Inferno::Texture m_fireballTex;
	Inferno::Idea m_fireball_;
	Inferno::Bullet* m_fireball;

	Inferno::Score<int> m_score;
};