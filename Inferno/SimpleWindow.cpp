#include<SimpleWindow.h>
#include<Input.h>
#include"MyException.h"
#include"TString.h"
#include"MyTypes.h"

//���������[�N���o�p�B
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#endif

namespace
{
	LPCTSTR className = TEXT("SimpleWindow");

	LRESULT WINAPI SimpleWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

namespace Inferno
{
SimpleWindow::SimpleWindow()
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif //_DEBUG

	//::SetProcessDPIAware();

	//WindowClass�ݒ�
	m_wc.cbSize = sizeof(WNDCLASSEX);
	m_wc.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW; //CS_CLASSDC:�S�ẴE�B���h�E�������f�o�C�X�R���e�L�X�g�����L����
	m_wc.lpfnWndProc = SimpleWindowProc;
	m_wc.cbClsExtra = 0L;
	m_wc.cbWndExtra = 0L;
	m_wc.hInstance = GetModuleHandle(nullptr);
	m_wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	m_wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	m_wc.lpszMenuName = nullptr;
	m_wc.lpszClassName = className;
	m_wc.hIconSm = nullptr;

	//WindowClass�o�^
	if (!RegisterClassEx(&m_wc))
	{
		throw Inferno::CreationFailed(TEXT("�E�B���h�E�N���X�̓o�^�Ɏ��s���܂����B"));
	}

	//�E�B���h�E�𐶐�
	m_hWnd = CreateWindow(className, TEXT("SimpleWindow"),
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, //|WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, //�E�B���h�E�ʒu
		CW_USEDEFAULT, CW_USEDEFAULT, //�E�B���h�E�T�C�Y
		GetDesktopWindow(), //�����͒���
		nullptr, m_wc.hInstance, nullptr);
	if (m_hWnd == nullptr)
	{
		throw Inferno::CreationFailed(TEXT("�E�B���h�E�̐����Ɏ��s���܂����B"));
	}

	this->Resize(800, 600);
}

SimpleWindow::~SimpleWindow()
{
	::UnregisterClass(className, m_wc.hInstance);

	::_CrtDumpMemoryLeaks();
}

void SimpleWindow::Resize(const int width, const int height)
{
	// �E�B���h�E�T�C�Y���Đݒ肷��
	RECT rect;
	int ww, wh;
	int cw, ch;
	// �N���C�A���g�̈�̊O�̕����v�Z
	GetClientRect(m_hWnd, &rect);		// �N���C�A���g�����̃T�C�Y�̎擾
	cw = rect.right - rect.left;	// �N���C�A���g�̈�O�̉������v�Z
	ch = rect.bottom - rect.top;	// �N���C�A���g�̈�O�̏c�����v�Z

	// �E�C���h�E�S�̂̉����̕����v�Z
	GetWindowRect(m_hWnd, &rect);		// �E�C���h�E�S�̂̃T�C�Y�擾
	ww = rect.right - rect.left;	// �E�C���h�E�S�̂̕��̉������v�Z
	wh = rect.bottom - rect.top;	// �E�C���h�E�S�̂̕��̏c�����v�Z
	ww = ww - cw;					// �N���C�A���g�̈�ȊO�ɕK�v�ȕ�
	wh = wh - ch;					// �N���C�A���g�̈�ȊO�ɕK�v�ȍ���

	// �E�B���h�E�T�C�Y�̍Čv�Z
	ww = width + ww;			// �K�v�ȃE�C���h�E�̕�
	wh = height + wh;		// �K�v�ȃE�C���h�E�̍���

	// �E�C���h�E�T�C�Y�̍Đݒ�
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	m_windowHeight = wh;
	m_windowWidth = ww;
	m_clientHeight = height;
	m_clientWidth = width;
}

void SimpleWindow::SetWindowAlignment(const HorizontalAlignment h, const VerticalAlignment v)
{
	//GetWindowPlacement��SetWindowPlacement��
	//�ʒu�̕ۑ��ƕ������ł���

	// �E�C���h�E�S�̂̉����̕����v�Z
	RECT rect;
	GetWindowRect(m_hWnd, &rect);		// �E�C���h�E�S�̂̃T�C�Y�擾
	const int ww = rect.right - rect.left;	// �E�C���h�E�S�̂̕��̉������v�Z
	const int wh = rect.bottom - rect.top;	// �E�C���h�E�S�̂̕��̏c�����v�Z

	//result
	int rx = 0;
	int ry = 0;

	//�ʒu�����W�ɒ���
	switch(h)
	{
	case HorizontalAlignment::Center:
		rx = (GetSystemMetrics(SM_CXSCREEN) - ww) / 2;
		if (rx < 0) rx=0; //��ʂ������������ꍇ
		break;
	default:
		assert(false);
		break;
	}

	switch(v)
	{
	case VerticalAlignment::Center:
		ry = (GetSystemMetrics(SM_CYSCREEN) - wh) / 2;
		if (ry < 0) ry = 0; //��ʂ������������ꍇ
		break;
	default:
		assert(false);
		break;
	}

	//adjust (offset)
	int ax = 0;
	int ay = 0;

	//�^�X�N�o�[���������ꍇ��������
	//�^�X�N�o�[�̈ʒu����`�œ���
	APPBARDATA sBarInfo;
	ZeroMemory(&sBarInfo, sizeof(APPBARDATA));
	sBarInfo.cbSize = sizeof(APPBARDATA);
	sBarInfo.hWnd = m_hWnd;
	SHAppBarMessage(ABM_GETTASKBARPOS, &sBarInfo);
	RECT r = sBarInfo.rc;
	//�^�X�N�o�[�����ɂ��������̑Ώ�
	if (r.left <= 0 && r.top <= 0 &&//�^�X�N�o�[�̍��[�Ə�[��0�ȉ���
		r.right <= r.bottom //�E�ӂ�艺�ӂ������ꍇ
		)
	{
		ax = r.right; //�^�X�N�o�[�������̈�����炷
	}
	//�����f�X�N�g�b�v���I�[�o�[�����ꍇ��
	if (rx + ax + ww > GetSystemMetrics(SM_CXSCREEN))
	{
		//�^�X�N�o�[�A�������̓f�X�N�g�b�v�[�ɃW���X�g�����悤�ɂ���
		rx = ax - (ww - m_clientWidth)/2;  //���̒l�̓E�B���h�E�g�����Y�����Ă���
		ax = 0;
	}

	//�^�X�N�o�[����ɂ��������̑Ώ�
	if (r.left <= 0 && r.top <= 0 &&//�^�X�N�o�[�̍��[�Ə�[��0�ȉ���
		r.bottom <= r.right //���ӂ��E�ӂ������ꍇ
		)
	{
		ay = r.bottom;//�^�X�N�o�[�������̈�����炷
	}
	//�����f�X�N�g�b�v���I�[�o�[�����ꍇ��
	if (ry + ay + wh > GetSystemMetrics(SM_CYSCREEN))
	{
		ry = ay - (wh - m_clientHeight)/2; //�^�X�N�o�[�A�������̓f�X�N�g�b�v�[�ɃW���X�g�����悤�ɂ���
		ay = 0;
	}
	

	::SetWindowPos(m_hWnd, HWND_TOP, rx+ax, ry+ay, 0, 0, SWP_NOSIZE);
}

void SimpleWindow::Show() const
{
	::ShowWindow(m_hWnd, SW_NORMAL);
	::ValidateRect(m_hWnd, nullptr);
	::UpdateWindow(m_hWnd); //WM_PAINT���b�Z�[�W���o���āA�̈���X�V
}

void SimpleWindow::SetTitle(LPCTSTR str) const
{
	::SetWindowText(m_hWnd, str);
}


HWND SimpleWindow::GetHWnd() const
{
	return m_hWnd;
}

int SimpleWindow::GetWindowWidth() const noexcept
{
	return m_windowWidth;
}

int SimpleWindow::GetWindowHeight() const noexcept
{
	return m_windowHeight;
}

int SimpleWindow::GetClientWidth() const noexcept
{
	return m_clientWidth;
}

int SimpleWindow::GetClientHeight() const noexcept
{
	return m_clientHeight;
}

bool SimpleWindow::Terminate()
{
	::DestroyWindow(m_hWnd);

	return true;
}

}

namespace
{
LRESULT WINAPI SimpleWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
}

