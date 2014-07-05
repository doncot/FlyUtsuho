#pragma once
#include"MyTypes.h"
#include<string>
#include"Graphics.h"

namespace Inferno
{
class DXText{
public:
	DXText();
	void Initialize(const Graphics& g, int point=28, const std::wstring& font=L"MS �S�V�b�N");
	int Print(const Graphics& g,const std::wstring& str, int x, int y);
	int Print(const Graphics& g,const std::wstring& str, Rect* rect);

private:
	LPD3DXFONT  m_font;		//�t�H���g�f�o�C�X
	Rect m_rect; //�`��͈�
	Matrix m_matrix;

};

}