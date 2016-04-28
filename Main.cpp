#include<Windows.h>
#include<tchar.h>

#include"SVShooter.h"

using namespace Inferno;

INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	SVShooter game;

	game.Startup();
	game.SetTitle(_T("フライ・お空ちゃん・フライ！"));
	game.Show();

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
		{
			//WM_QUITEがきたらGetMessageは0を返す
			if (!GetMessage(&msg, nullptr, 0, 0)) break;

			TranslateMessage(&msg); //キーボード関連のメッセージを翻訳する
			DispatchMessage(&msg); //OSにメッセージを渡す（＝ウィンドウプロシージャに渡す）
		}
		else
		{
			//ゲームループ
			game.GameLoop();
		}
		Sleep(1);
	}

	return msg.wParam; //作法
}