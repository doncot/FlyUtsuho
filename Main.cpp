#include<Windows.h>
#include<tchar.h>

#include"SVShooter.h"

using namespace Inferno;

INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	SVShooter game;

	game.Startup();
	game.SetTitle(_T("�t���C�E���󂿂��E�t���C�I"));
	game.Show();

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
		{
			//WM_QUITE��������GetMessage��0��Ԃ�
			if (!GetMessage(&msg, nullptr, 0, 0)) break;

			TranslateMessage(&msg); //�L�[�{�[�h�֘A�̃��b�Z�[�W��|�󂷂�
			DispatchMessage(&msg); //OS�Ƀ��b�Z�[�W��n���i���E�B���h�E�v���V�[�W���ɓn���j
		}
		else
		{
			//�Q�[�����[�v
			game.GameLoop();
		}
		Sleep(1);
	}

	return msg.wParam; //��@
}