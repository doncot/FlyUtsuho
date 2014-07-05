#include<DXText.h>

#include<tchar.h>
#include<d3dx9.h>

#include"Common.h"
#include"MyException.h"
#include"TString.h"

namespace Inferno
{
DXText::DXText()
: m_font(nullptr)
{
	MatrixIdentity(&m_matrix);
}

void DXText::Initialize(const Graphics& g, int point, const std::wstring& font)
{
	D3DXFONT_DESC desc;	//font�̑���
	desc.Height = point;
	desc.Width = 0;
	desc.Weight = FW_NORMAL; //����
	desc.MipLevels = 0;
	desc.Italic = false;
	desc.CharSet = DEFAULT_CHARSET;
	desc.OutputPrecision = OUT_OUTLINE_PRECIS;
	desc.Quality = PROOF_QUALITY;
	desc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE; //FIXED_PITCH | FF_MODERN
	lstrcpyW(desc.FaceName, font.c_str());

	// ID3DXFont�R���|�[�l���g����
	HRESULT hr = D3DXCreateFontIndirect(g.GetDevice(), &desc, &m_font);
	if (FAILED(hr)){
		throw CreationFailed(TEXT("�t�H���g�I�u�W�F�N�g"));
	}
}
//=================================================
//�e�L�X�g��x,y�ɏo��
//���s���ɂ�0���A�������ɂ̓e�L�X�g�̍�����߂�
//���s�O�Fspritebegin
//���s��Fspriteend
int DXText::Print(const Graphics& g,const std::wstring& str, int x, int y)
{
	if (!m_font) return 0;

	m_rect.AMove(x, y);

	MatrixIdentity(&m_matrix);
	g.TransformSprite(&m_matrix);
	return m_font->DrawTextW(g.GetSprite(), str.c_str(), -1, &m_rect.GetMSRECT(), DT_LEFT|DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}

int DXText::Print(const Graphics& g,const std::wstring& str, Rect* rect)
{
	if (!m_font) return 0;

	MatrixIdentity(&m_matrix);
	g.TransformSprite(&m_matrix);
	return m_font->DrawTextW(g.GetSprite(), str.c_str(), -1, &rect->GetMSRECT(), DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}

/*
void DXText::Draw(const std::string& str, const int len)
{
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	GraphicsDirectX::m_sprite->SetTransform(&matrix);
	if (SUCCEEDED(GraphicsDirectX::m_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE)))
	{
		m_font->DrawText(GraphicsDirectX::m_sprite, TString(str), len, &m_rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}
	GraphicsDirectX::m_sprite->End();
}
*/

}