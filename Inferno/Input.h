#pragma once
#include<Windows.h>

namespace InputNS {
	const int KeyLen = 256;
}

namespace Inferno
{
class Input
{
public:
	Input();

	int GetMouseX() const;
	int GetMouseY() const;
	bool IsMouseLButtonPressed() const;
	bool IsKeyDown(const unsigned char vkey) const;
	//IskeyPressed���^�ɂȂ����ꍇ�A�Ăяo����Y���L�[�̓��Z�b�g�����B
	bool CheckKeyPressed(const unsigned char vkey);
	//IsKeyPressed��const�o�[�W�����B���Z�b�g���Ȃ�
	bool IsKeyPressed(const unsigned char vkey) const;
	bool IsAnyKeyPressed() const;

	void ClearMouseLButton();
	void ClearKeys();

	void DisableIME(HWND hWnd);
	void EnableIME(HWND hWnd);

	bool InputProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	//�}�E�X�֘A
	bool m_LButtonDown;
	bool m_LButtonPressed;
	int m_mouseX;
	int m_mouseY;
	//�L�[�{�[�h�֘A
	bool m_keysDown[InputNS::KeyLen];
	bool m_keysPressed[InputNS::KeyLen];

	//���̓R���e�L�X�g�n���h���iIME�𖳌��ɂ���ەۑ����Ă����A�ĂїL���ɖ߂��ۂɎg���j
	HIMC m_hIMC;
};

}