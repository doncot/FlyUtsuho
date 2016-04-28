/*
ゲーム用のウィンドウをクラス化したもの
*/
#pragma once
#include<Common.h>

namespace Inferno
{
	//定数
	enum class HorizontalAlignment
	{
		Left,
		Center,
		Right,
	};

	enum class VerticalAlignment
	{
		Top,
		Center,
		Bottom,
	};

class SimpleWindow
{
public:
	SimpleWindow();
	virtual ~SimpleWindow() noexcept;

	void Resize(const int width, const int height);

	///<summary>
	///現在の位置とサイズでウィンドウをアクティブにする
	///</summary>
	void Show() const;
	void SetTitle(LPCTSTR str) const;
	void SetWindowAlignment(const HorizontalAlignment h, const VerticalAlignment v);

	HWND GetHWnd() const;
	int GetWindowWidth() const noexcept;
	int GetWindowHeight() const noexcept;
	int GetClientWidth() const noexcept;
	int GetClientHeight() const noexcept;
	
	///<summary>
	///ウィンドウを終了させる
	///</summary>
	virtual bool Terminate();

private:
	WNDCLASSEX m_wc;
	HWND m_hWnd; //自身を指すウィンドウハンドル

	//ウィンドウ枠を含めた領域（ウィンドウ領域）
	int m_windowWidth;
	int m_windowHeight;

	//ウィンドウ枠を除いた領域(クライアント領域)
	int m_clientWidth;
	int m_clientHeight;
	
private:
	SimpleWindow(SimpleWindow& w) = delete;
	SimpleWindow operator=(SimpleWindow& w) = delete;
};
}