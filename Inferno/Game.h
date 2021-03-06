/*
Game.h
ゲーム本体
これは基底で各ゲームごとに派生させる
*/
#pragma once
#include"SimpleWindow.h"
#include"Graphics.h"
#include"Input.h"
#include"Timer.h"

namespace Inferno
{
	class Graphics;
	class Input;
	class Timer;

	class Game : public Inferno::SimpleWindow
	{
	public:
		Game();
		virtual ~Game() override;
		void SetClientSize(const int width, const int height);

		virtual bool GameLoop();
		virtual void Exit();

	protected:
		Inferno::Graphics m_graphics;
		Inferno::Input m_input;

		Inferno::Timer m_globalTimer; //ゲーム開始時にスタート

		int m_elapsedFrame; //ゲーム開始からの経過フレーム
		int m_frameCount; //前回のループからの経過フレーム

	private:
		typedef Inferno::SimpleWindow Base;
	};
}