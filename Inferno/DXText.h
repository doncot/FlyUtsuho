#pragma once
#include"MyTypes.h"
#include<string>
#include"Graphics.h"

namespace Inferno
{
class DXText{
public:
	DXText();
	void Initialize(const Graphics& g, int point=28, const std::wstring& font=L"MS ゴシック");
	int Print(const Graphics& g,const std::wstring& str, int x, int y);
	int Print(const Graphics& g,const std::wstring& str, Rect* rect);

private:
	LPD3DXFONT  m_font;		//フォントデバイス
	Rect m_rect; //描画範囲
	Matrix m_matrix;

};

}