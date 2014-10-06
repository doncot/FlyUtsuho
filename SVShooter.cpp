#include"SVShooter.h"

#include<Texture.h>
#include<GameElements.h>
#include"Common.h"
#include<Collision.h>
#include"Audio.h"
#include<Timer.h>
#include"MyException.h"
#include<DXText.h>
#include"TString.h"
#include<vector>
#include<cmath>
#include<sstream>
#include"ShooterElements.h"
#include"Interpreter.h"

#define PI 3.14159265358979323846

namespace
{
	//scene
	Inferno::ShooterScene scene;

	//リソースマネージャー（今の所シングルトン[つまりグローバル]）
	Inferno::ResourceManager* resourceMan;

	//image
	std::vector<Inferno::Instant> bg_underground;
	Inferno::Texture purpleBulletTex;
	Inferno::Idea purpleBullet_;
	std::list<Inferno::Substance*> purpleBullets;
	Inferno::Texture eggTex;
	Inferno::Idea egg_;
	Inferno::Substance egg;

	//Audio
	Inferno::AudioMaster audio;
	Inferno::Audio bgm;
	Inferno::Audio fire;
	Inferno::Audio pichun;

	//text
	Inferno::DXText scoreText;
	Inferno::DXText dLight;
}

SVShooter::SVShooter()
{
	//親コンストラクタは自動で呼ばれるのでわざわざ呼ぶ必要はない
}

SVShooter::~SVShooter()
{
	SAFE_DELETE(m_utsuho);
	SAFE_DELETE(m_fireball);
	for (auto i = purpleBullets.begin(); i != purpleBullets.end();)
	{
		SAFE_DELETE(*i);
		i = purpleBullets.erase(i);
	}

	SAFE_DELETE(resourceMan);
}

bool SVShooter::Initialize()
{
	try
	{
		Base::Initialize();
		m_screenRect.SetSize(GetClientWidth(),GetClientHeight());
		m_screenRect.SetPosofULCorner(0, 0);

		m_titleImage.Initialize();
		m_titleImage.LoadTextureFromFile(Base::m_graphics, TEXT("Sprites\\intro.png"));
		m_titleImage.SetPosofULCorner(0, 0);

		//ゲーム要素の初期化
		resourceMan = new Inferno::ResourceManager(m_graphics);
		resourceMan->SetBullet(L"redbullet", L"Sprites\\fireball.png", this->m_screenRect, 50);

		m_utsuhoTex.LoadImageFile(Base::m_graphics, TEXT("Sprites\\utsuho.png"));
		m_utsuho_.SetTexture(m_utsuhoTex);
		m_utsuho = new Inferno::Player();
		m_utsuho->SetIdea(m_utsuho_);
		m_utsuho->AMove(200, 200);
		m_utsuho->SetMoveLimit(Inferno::Rect(700, 600));
		m_utsuho->SetAttribute(Inferno::GEAttribute::Visible,true);

		m_fireballTex.LoadImageFile(Base::m_graphics, TEXT("Sprites\\fireball.png"));
		m_fireball_.SetTexture(m_fireballTex);
		m_fireball = nullptr;

		purpleBulletTex.LoadImageFile(m_graphics, _T("Sprites\\purplebullet.png"));
		purpleBullet_.SetTexture(purpleBulletTex);

		bg_underground.resize(2);
		for (auto e = bg_underground.begin(); e != bg_underground.end();e++)
		{
			e->Initialize();
			e->LoadTextureFromFile(m_graphics, TEXT("Sprites\\underground.png"));
		}
		bg_underground[0].SetPosofULCorner(0, 0);
		bg_underground[1].SetPosofULCorner(800, 0);

		eggTex.LoadImageFile(m_graphics,_T("Sprites\\egg.bmp"));
		egg_.SetTexture(eggTex);
		egg.SetIdea(egg_);
		egg.AMove(800, 300);

		//最初のシーン
		m_scene = sn_title;

		//サウンド
		audio.Initialize();
		bgm.Initialize(&audio);
		fire.Initialize(&audio);
		pichun.Initialize(&audio);
		bgm.LoadWaveFile(_T("BGM\\Nuclear_Fusion.wav"));
		fire.LoadWaveFile(_T("SE\\sensescircuit\\pui.wav"));
		pichun.LoadWaveFile(_T("SE\\niconicommons\\nc899.wav"));
		pichun.SetVolume(0.3f);

		//テキスト
		scoreText.Initialize(m_graphics);
		dLight.Initialize(m_graphics);

		//スクリプト
		scene.InitializeScene(m_graphics,this->m_screenRect);
		Inferno::Scripter::LoadSceneFromScript(&scene,_T("Script\\TestScene_rev2.script"));
	}
	catch (const Inferno::MyExceptionBase_RuntimeError& e)
	{
		MessageBoxW(GetHWND(), e.what_w(), L"初期化失敗", MB_OK);
	}
	catch (const exception& e)
	{
		MessageBoxW(GetHWND(), TString(e.what()).GetStringW(), L"初期化失敗", MB_OK);
	}
	return true;
}

bool SVShooter::GameLoop()
{
	Base::GameLoop();
	static Inferno::Timer sceneTimer;

	switch (m_scene)
	{
	case sn_title:
		//BGM
		//bgm.Play();

		//入力
		if (m_input.IsAnyKeyPressed() == true || m_input.IsMouseLButtonPressed() == true )
		{
			m_scene = sn_main;
			m_input.ClearKeys();
			m_input.ClearMouseLButton();
		}

		//描画
		if (Base::m_graphics.BeginScene() && Base::m_graphics.BeginSprite())
		{
			m_titleImage.Draw(m_graphics);
		}
		Base::m_graphics.EndSprite();
		Base::m_graphics.EndScene();

		break;
	case sn_main:
		if (!sceneTimer.HasStarted()) sceneTimer.Start();
		if (!scene.HasStarted()) scene.Start();
		scene.Update();


		//入力
		const int utsuhoVel = 6;
		if (m_input.IsKeyDown(VK_UP))
		{
			m_utsuho->RMove(0, -utsuhoVel);
		}
		if (m_input.IsKeyDown(VK_DOWN))
		{
			m_utsuho->RMove(0, utsuhoVel);
		}
		if (m_input.IsKeyDown(VK_LEFT))
		{
			m_utsuho->RMove(-utsuhoVel, 0);
		}
		if (m_input.IsKeyDown(VK_RIGHT))
		{
			m_utsuho->RMove(utsuhoVel, 0);
		}
		if (m_input.IsKeyPressed('Z'))
		{
			m_utsuho->Shoot(0,8);
			fire.Stop();
			fire.Play();
		}

		//更新処理（移動・アニメーション）
		//スコア
		m_score.Update();

		//自機
		m_utsuho->Update();

		//背景
		for (auto e = bg_underground.begin(); e != bg_underground.end(); e++)
		{
			e->RMove(-1, 0);
		}
		//敵の弾
		for (auto e = purpleBullets.begin(); e != purpleBullets.end(); e++)
		{
			(*e)->Update();
		}

		static Inferno::Stopwatch eggInterval(6000);
		//卵
		if (eggInterval.HasFinished())
		{
			egg.AMove(800, 300);
			eggInterval.Restart();
			egg.SetAttribute(Inferno::GEAttribute::Visible,true);
		}
		else
		{
			static int angle = 0;
			egg.RMove(-4, 0);
			egg.Rotate(angle += 3);
		}

		//衝突判定**************************************************************
		//敵弾と自機の衝突判定
		if (scene.ProcessEnemyBulletToPlayerHit(m_utsuho->GetHitBox()))
		{
			//被弾SE
			pichun.Stop();
			pichun.Play();
			m_score -= 2500;
					
			//m_utsuho->RegisterDamage();
		}

		//自機と敵機の衝突判定
		if (scene.ProcessPlayerToEnemyHit(m_utsuho->GetHitBox()))
		{
			//被弾SE
			pichun.Stop();
			pichun.Play();
			m_score -= 2500;

			//m_utsuho->RegisterDamage();
		}

		//卵と自機の衝突検出
		if (Inferno::IsRect1HittingRect2(m_utsuho->GetHitBox(), egg.GetHitBox()))
		{
			if (egg.CheckAttribute(Inferno::GEAttribute::Visible))
			{
				m_score += 1000;
				eggInterval.Restart();
				egg.SetAttribute(Inferno::GEAttribute::Visible, false);
			}
		}

		//自機弾と敵の衝突処理
		{
			auto bulletList = m_utsuho->GetBulletList();
			for (auto bullet_i = bulletList.begin(); bullet_i != bulletList.end();)
			{
				if (scene.ProcessPlayerBulletToEnemyHit(**bullet_i))
				{
					//当たっていた場合
					//弾を管理するplayerに該当の弾を渡し、消してもらう
					//消す前に次弾をキープ
					auto nextBullet = bullet_i;
					nextBullet++;
					m_utsuho->EraseGivenBullet(**bullet_i);
					//!!!!注意!!!!:この時点でこのbulletは無効。
					//TODO:こういうときこそmoveでは？
					bullet_i = nextBullet;
					continue;
				}
				bullet_i++;
			}
		}

		/*
		//シーン切り替え
		if (!m_onryouList[0] && !m_onryouList[1])
		{
			MessageBox(nullptr, L"殲滅完了！", L"メッセージ", MB_OK);
			this->Exit();
		}
		*/

		
		//描画
		if (Base::m_graphics.BeginScene() && Base::m_graphics.BeginSprite())
		{
			for (auto& e : bg_underground)
			{
				e.Draw(m_graphics);
			}

			m_utsuho->Draw(m_graphics);

			if (m_fireball)	m_fireball->Draw(m_graphics);

			for (auto& e: purpleBullets)
			{
				e->Draw(m_graphics);
			}

			egg.Draw(m_graphics);

			//シーン
			scene.Draw();

			//スコア
			wstringstream ss;
			//ss << L"Existing enemy count: " << m_.size();
			scoreText.Print(m_graphics, _T("スコア：") + m_score.DisplayScore() , 50, 540);
			//dLight.Print(m_graphics, ss.str() , 500, 30);
		}
		Base::m_graphics.EndSprite();
		Base::m_graphics.EndScene();
		

		break;
	}

	return true;
}
