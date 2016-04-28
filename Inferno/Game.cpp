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

	//�E�B���h�E�̏�����
	Base::Resize(800, 600);
	Base::SetWindowAlignment(HorizontalAlignment::Center, VerticalAlignment::Center);
	Base::Show();

	//�E�B�h�E�v���V�[�W������������
	::SetWindowLongPtr(GetHWnd(), GWLP_WNDPROC, (LONG_PTR)MessageHandler);

	//IME������
	m_input.DisableIME(GetHWnd());

	//�O���t�B�b�N�X�̏�����
	m_graphics.Initialize(GetHWnd());

	//�^�C�}�[���X�^�[�g
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
	//�t���[���Ǘ�
	static int previousFrame = 0;
	m_frameCount = m_elapsedFrame - previousFrame;
	m_elapsedFrame++;
	previousFrame = m_elapsedFrame;

	//Esc�L�[�ŃQ�[���I��
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
			//����
			return TRUE;
		default:
			break;
		}

		//���͂�����
		InputHandle->InputProc(hWnd, msg, wParam, lParam);

		return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
}