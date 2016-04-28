#include<Game.h>
#include"Graphics.h"

namespace
{
	Inferno::Input* InputHandle;
	LRESULT WINAPI MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

using namespace Inferno;

Game::Game() :
	 m_elapsedFrame(0), m_frameCount(0)
{
	InputHandle = &m_input;

	//ウィンドウの初期化
	Base::Resize(800, 600);
	Base::SetWindowAlignment(HorizontalAlignment::Center, VerticalAlignment::Center);
	Base::Show();

	//ウィドウプロシージャを書き換え
	::SetWindowLongPtr(GetHWnd(), GWLP_WNDPROC, (LONG_PTR)MessageHandler);

	//IME無効化
	m_input.DisableIME(GetHWnd());

	//グラフィックスの初期化
	m_graphics.Initialize(GetHWnd());

	//タイマーをスタート
}

Game::~Game()
{
	m_graphics.Finalize();

	InputHandle = nullptr;
}

void Game::SetClientSize(const int width, const int height)
{
	Base::Resize(width,height);
	Base::SetWindowAlignment(HorizontalAlignment::Center, VerticalAlignment::Center);
	m_graphics.DisplayBlankScreen(0, 0, 128);
}

bool Game::GameLoop()
{
	//フレーム管理
	static int previousFrame = 0;
	m_frameCount = m_elapsedFrame - previousFrame;
	m_elapsedFrame++;
	previousFrame = m_elapsedFrame;

	//Escキーでゲーム終了
	if (m_input.IsKeyPressed(VK_ESCAPE))
	{
		Exit();
		return false;
	}


	return true;
}

void Game::Exit()
{
	Base::Terminate();
}

namespace
{
	LRESULT WINAPI MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_ERASEBKGND:
			//無視
			return TRUE;
		default:
			break;
		}

		//入力を処理
		InputHandle->InputProc(hWnd, msg, wParam, lParam);

		return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
}