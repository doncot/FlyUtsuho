#include"SimpleWindow.h"

//メモリリーク検出用。
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
///ウィンドウスタイルの値を保持
///AdjustWindowRectにてバグがあってWS_CAPTIONを設定する必要がある（WS_SYSMENUに含まれるので冗長なはずだが、計算に入れてない）
const DWORD SimpleWindow::m_windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX; //|WS_MAXIMIZEBOX,

SimpleWindow::SimpleWindow()
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif //_DEBUG

	//::SetProcessDPIAware();

	//WindowClass設定
	m_wc.cbSize = sizeof(WNDCLASSEX);
	//CS_CLASSDC:全てのウィンドウが同じデバイスコンテキストを共有する
	m_wc.style = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW;
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

	//WindowClass登録
	if (!RegisterClassEx(&m_wc))
	{
		throw std::runtime_error("ウィンドウクラスの登録に失敗しました。");
	}

	//ウィンドウを生成
	m_hWnd = CreateWindow(className, TEXT("SimpleWindow"),
		m_windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, //ウィンドウ位置
		CW_USEDEFAULT, CW_USEDEFAULT, //ウィンドウサイズ
		nullptr, nullptr, m_wc.hInstance, nullptr);
	if (m_hWnd == nullptr)
	{
		throw std::runtime_error("ウィンドウの生成に失敗しました。");
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
	RECT rect = {0, 0, width, height};
	::AdjustWindowRect(&rect, m_windowStyle, false);

	const int windowWidth = rect.right - rect.left;
	const int windowHeight = rect.bottom - rect.top;

	//// ウィンドウサイズを再設定する
	//RECT rect;
	//int ww, wh;
	//int cw, ch;
	//// クライアント領域の外の幅を計算
	//GetClientRect(m_hWnd, &rect);		// クライアント部分のサイズの取得
	//cw = rect.right - rect.left;	// クライアント領域外の横幅を計算
	//ch = rect.bottom - rect.top;	// クライアント領域外の縦幅を計算

	//// ウインドウ全体の横幅の幅を計算
	//GetWindowRect(m_hWnd, &rect);		// ウインドウ全体のサイズ取得
	//ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
	//wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算
	//ww = ww - cw;					// クライアント領域以外に必要な幅
	//wh = wh - ch;					// クライアント領域以外に必要な高さ

	//// ウィンドウサイズの再計算
	//ww = width + ww;			// 必要なウインドウの幅
	//wh = height + wh;		// 必要なウインドウの高さ

	// ウインドウサイズの再設定
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_NOMOVE);

	m_windowWidth = windowWidth;
	m_windowHeight = windowHeight;
	m_clientWidth = width;
	m_clientHeight = height;
}

void SimpleWindow::SetWindowAlignment(HorizontalAlignment h, VerticalAlignment v)
{
	//GetWindowPlacementとSetWindowPlacementで
	//位置の保存と復元ができる

	// ウインドウ全体の横幅の幅を計算
	RECT rect;
	GetWindowRect(m_hWnd, &rect);		// ウインドウ全体のサイズ取得
	const int ww = rect.right - rect.left;	// ウインドウ全体の幅の横幅を計算
	const int wh = rect.bottom - rect.top;	// ウインドウ全体の幅の縦幅を計算

	//result
	int rx = 0;
	int ry = 0;

	//位置を座標に直す
	switch(h)
	{
	case HorizontalAlignment::Center:
		rx = (GetSystemMetrics(SM_CXSCREEN) - ww) / 2;
		if (rx < 0) rx=0; //画面が小さすぎた場合
		break;
	default:
		assert(false);
		break;
	}

	switch(v)
	{
	case VerticalAlignment::Center:
		ry = (GetSystemMetrics(SM_CYSCREEN) - wh) / 2;
		if (ry < 0) ry = 0; //画面が小さすぎた場合
		break;
	default:
		assert(false);
		break;
	}

	//adjust (offset)
	int ax = 0;
	int ay = 0;

	//タスクバーがあった場合調整する
	//タスクバーの位置を矩形で入手
	APPBARDATA sBarInfo;
	ZeroMemory(&sBarInfo, sizeof(APPBARDATA));
	sBarInfo.cbSize = sizeof(APPBARDATA);
	sBarInfo.hWnd = m_hWnd;
	SHAppBarMessage(ABM_GETTASKBARPOS, &sBarInfo);
	RECT r = sBarInfo.rc;
	//タスクバーが左にあった時の対処
	if (r.left <= 0 && r.top <= 0 &&//タスクバーの左端と上端が0以下で
		r.right <= r.bottom //右辺より下辺が長い場合
		)
	{
		ax = r.right; //タスクバー分だけ領域を減らす
	}
	//もしデスクトップをオーバーした場合は
	if (rx + ax + ww > GetSystemMetrics(SM_CXSCREEN))
	{
		//タスクバー、もしくはデスクトップ端にジャストくっつようにする
		rx = ax - (ww - m_clientWidth)/2;  //後ろの値はウィンドウ枠だけズラしている
		ax = 0;
	}

	//タスクバーが上にあった時の対処
	if (r.left <= 0 && r.top <= 0 &&//タスクバーの左端と上端が0以下で
		r.bottom <= r.right //下辺より右辺が長い場合
		)
	{
		ay = r.bottom;//タスクバー分だけ領域を減らす
	}
	//もしデスクトップをオーバーした場合は
	if (ry + ay + wh > GetSystemMetrics(SM_CYSCREEN))
	{
		ry = ay - (wh - m_clientHeight)/2; //タスクバー、もしくはデスクトップ端にジャストくっつようにする
		ay = 0;
	}
	

	::SetWindowPos(m_hWnd, HWND_TOP, rx+ax, ry+ay, 0, 0, SWP_NOSIZE);
}

void SimpleWindow::Show() const
{
	::ShowWindow(m_hWnd, SW_NORMAL);
	::ValidateRect(m_hWnd, nullptr);
	::UpdateWindow(m_hWnd); //WM_PAINTメッセージを出して、領域を更新
}

void SimpleWindow::SetTitle(LPCTSTR str) const
{
	::SetWindowText(m_hWnd, str);
}


HWND SimpleWindow::GetHWnd() const noexcept
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

void SimpleWindow::Terminate()
{
	::DestroyWindow(m_hWnd);
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

