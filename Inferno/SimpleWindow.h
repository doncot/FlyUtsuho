/*
�Q�[���p�̃E�B���h�E���N���X����������
*/
#pragma once
#include<Common.h>

namespace Inferno
{
	///���������̔z�u
	enum class HorizontalAlignment
	{
		Left,
		Center,
		Right,
	};

	///���������̔z�u
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
	///���݂̈ʒu�ƃT�C�Y�ŃE�B���h�E���A�N�e�B�u�ɂ���
	///</summary>
	void Show() const;
	void SetTitle(LPCTSTR str) const;
	void SetWindowAlignment(HorizontalAlignment h, VerticalAlignment v);

	HWND GetHWnd() const noexcept;

	int GetWindowWidth() const noexcept;
	int GetWindowHeight() const noexcept;
	int GetClientWidth() const noexcept;
	int GetClientHeight() const noexcept;
	
	///<summary>
	///�E�B���h�E���I��������
	///</summary>
	virtual void Terminate();

private:
	WNDCLASSEX m_wc;
	HWND m_hWnd; //���g���w���E�B���h�E�n���h��

	//�E�B���h�E�g���܂߂��̈�i�E�B���h�E�̈�j
	int m_windowWidth;
	int m_windowHeight;

	//�E�B���h�E�g���������̈�(�N���C�A���g�̈�)
	int m_clientWidth;
	int m_clientHeight;

	//�萔
	///�E�B���h�E�X�^�C��
	static const DWORD m_windowStyle;
	
private:
	SimpleWindow(SimpleWindow& w) = delete;
	SimpleWindow operator=(SimpleWindow& w) = delete;
};
}