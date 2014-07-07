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

#define PI 3.14159265358979323846

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#endif  //_DEBUG

namespace
{
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
	for (auto e : m_onryouList)
	{
		SAFE_DELETE(e);
	}
	for (auto i = purpleBullets.begin(); i != purpleBullets.end();)
	{
		SAFE_DELETE(*i);
		i = purpleBullets.erase(i);
	}

}

bool SVShooter::Initialize()
{
	try
	{
		Base::Initialize();
		m_screenRect.SetSize(GetClientHeight(),GetClientWidth());
		m_screenRect.SetPosofULCorner(0, 0);

		//ゲーム要素の初期化
		m_titleImage.Initialize();
		m_titleImage.LoadTextureFromFile(Base::m_graphics, TEXT("Sprites\\intro.png"));
		m_titleImage.SetPosofULCorner(0, 0);

		m_utsuhoTex.LoadImageFile(Base::m_graphics, TEXT("Sprites\\utsuho.png"));
		m_utsuho_.SetTexture(m_utsuhoTex);
		m_utsuho = new Inferno::ShooterActor();
		m_utsuho->SetIdea(&m_utsuho_);
		m_utsuho->AMove(200, 200);

		m_fireballTex.LoadImageFile(Base::m_graphics, TEXT("Sprites\\fireball.png"));
		m_fireball_.SetTexture(m_fireballTex);
		m_fireball = nullptr;

		purpleBulletTex.LoadImageFile(m_graphics, _T("Sprites\\purplebullet.png"));
		purpleBullet_.SetTexture(purpleBulletTex);

		m_onryouTex.LoadImageFile(Base::m_graphics, TEXT("Sprites\\onryou.png"));
		m_onryou_.SetTexture(m_onryouTex);
		auto tsub = new Inferno::Substance(&m_onryou_);
		tsub->AMove(400, 300);
		m_onryouList.push_back(tsub);
		tsub = new Inferno::Substance(&m_onryou_);
		tsub->AMove(600, 400);
		m_onryouList.push_back(tsub);

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
		egg.SetIdea(&egg_);
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
		dLight.Initialize(m_graphics);
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
	if (!sceneTimer.HasStarted()) sceneTimer.Start();

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
			if (m_fireball == nullptr)
			{
				m_fireball = new Inferno::Bullet(&m_fireball_);
				m_fireball->Fire(m_utsuho->GetPosition(), 5, 0);

				fire.Stop();
				fire.Play();
			}
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
		//弾
		if (m_fireball) m_fireball->Update();
		for (auto e = purpleBullets.begin(); e != purpleBullets.end(); e++)
		{
			(*e)->Update();
		}

		//敵
		if (m_onryouList[0])
		{
			m_onryouList[0]->AMove(m_onryouList[0]->GetPosition().x,
				static_cast<const int>(200 + 200 * std::sin(PI * 2 / 60 * m_elapsedFrame / 3.0)));
			
			static Inferno::Stopwatch shootInterval(800);
			if (shootInterval.HasFinished())
			{
				auto born = new Inferno::Bullet(&purpleBullet_);
				born->Fire(m_onryouList[0]->GetPosition(), Inferno::Vec2<int>(-2, 0));
				purpleBullets.push_back(born);
				shootInterval.Restart();
			}
		}
		if (m_onryouList[1])
		{
			m_onryouList[1]->AMove(m_onryouList[1]->GetPosition().x,
				static_cast<const int>(300 + 125 * std::sin(PI * 2 / 60 * m_elapsedFrame / 3.5)));
		
		}

		static Inferno::Stopwatch eggInterval(6000);
		//卵
		if (eggInterval.HasFinished())
		{
			egg.AMove(800, 300);
			eggInterval.Restart();
			egg.Activate();
		}
		else
		{
			egg.RMove(-4, 0);
		}

		//衝突判定
		//弾
		if (m_fireball)
		{
			//画面外に出たら
			if (m_fireball->GetPosition().x > 750)
			{
				SAFE_DELETE(m_fireball);
			}
		}

		//敵弾が画面外に出たときに削除
		for (auto i = purpleBullets.begin(); i != purpleBullets.end(); )
		{
			if (!Inferno::IsPointInsideRect((*i)->GetPosition(), m_screenRect))
			{
				SAFE_DELETE(*i);
				i = purpleBullets.erase(i);
				continue;
			}
			i++;
		}
		

		//怨霊と火弾の衝突判定
		for (unsigned int i = 0; i < m_onryouList.size(); i++)
		{
			if (m_fireball && m_onryouList[i]
				&& Inferno::IsPointInsideRect(m_fireball->GetPosition(), m_onryouList[i]->GetRegion()))
			{
				SAFE_DELETE(m_fireball);
				SAFE_DELETE(m_onryouList[i]);
				break;
			}
		}

		//自機が画面外に出ないように
		Inferno::KeepSubInsideRect(m_utsuho, m_screenRect);

		//敵弾と自機の衝突判定
		for (auto e = purpleBullets.begin(); e != purpleBullets.end();)
		{
			//直撃
			if ( Inferno::IsPointInsideRect((*e)->GetPosition(), m_utsuho->GetRegion()) )
			{
				//弾を削除
				SAFE_DELETE(*e);
				e = purpleBullets.erase(e);
				//被弾SE
				pichun.Stop();
				pichun.Play();

				m_score -= 2500;
				
				m_utsuho->Hit();
				
				continue;
			}

			//次イテレーター
			e++;
		}

		//卵と自機の衝突検出
		if (Inferno::IsRect1HittingRect2(m_utsuho->GetRegion(), egg.GetRegion()))
		{
			if (egg.isActive())
			{
				m_score += 1000;
				eggInterval.Restart();
				egg.Deactivate();
			}
		}

		//シーン切り替え
		if (!m_onryouList[0] && !m_onryouList[1])
		{
			MessageBox(nullptr, L"殲滅完了！", L"メッセージ", MB_OK);
			this->Exit();
		}


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

			for (auto& e : m_onryouList)
			{
				if(e) e->Draw(m_graphics);
			}

			if (egg.isActive()) egg.Draw(m_graphics);

			//スコア
			dLight.Print(m_graphics, _T("スコア：") + m_score.DisplayScore() , 50, 540);
		}
		Base::m_graphics.EndSprite();
		Base::m_graphics.EndScene();

		break;
	}

	return true;
}
