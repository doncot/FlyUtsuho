#include<Texture.h>
#include"Graphics.h"
#include"MyException.h"
#include"Common.h"

using namespace std;

namespace Inferno{
Texture::Texture()
:m_tex(nullptr), m_width(0), m_height(0)
{
}

Texture::~Texture()
{
	SAFE_RELEASE(m_tex);
}

void Texture::LoadImageFile(const Graphics& g, const wstring& filename)
{
	m_texName = filename;
	g.LoadTexture(filename, &m_width, &m_height, &m_tex);
}
/*
void Texture::InitializeFromImage(const wstring& filename, int width, int height, int rowN)
{
	//ファイル名を覚えておく（デバッグ用とデバイスロスト用）
	m_texName = filename;

	HRESULT hr = D3DXCreateTextureFromFile(
		m_d3dDevice,
		TString(filename), &m_tex
		);
	if (hr == D3DXERR_INVALIDDATA)
	{
		throw FileNotFound(filename);
	}


	// テクスチャからサーフェイスを取得
	IDirect3DSurface9 *pSurface;
	m_tex->GetSurfaceLevel(0, &pSurface);   // サーフェイスを取得
	{//開放忘れの為のブロック

		// サーフェイス情報から画像サイズを取得
		D3DSURFACE_DESC SurfaceInfo;
		pSurface->GetDesc(&SurfaceInfo);

		if (width == 0 && height == 0 && (rowN == 0 || rowN == 1))
		{
			//テクスチャ全体を使う場合
			m_width = SurfaceInfo.Width;
			m_height = SurfaceInfo.Height;
		}
		else
		{
			//分割を行う

			//分割数×分割辺幅が元の画像と一致しないとき
			if (rowN!=0&&width*rowN!=SurfaceInfo.Width)
			{
				TString str;
				str = filename+L"(入力パラメータが不正)";
				throw CreationFailed(str);
			}

			//id=0は全体を示す特別な領域
			SubRegion sr;
			sr.id = 0; 
			sr.rect.left = sr.rect.top = 0;
			sr.rect.right = SurfaceInfo.Width;
			sr.rect.bottom = SurfaceInfo.Height;
			m_subRegions.push_back(sr);

			//分割テクスチャは1から
			for (int i = 0; i < rowN; i++)
			{
				SubRegion sr;
				sr.id = i+1; //既に0は入れている
				sr.rect.top = 0; //高さは0基準
				sr.rect.bottom = SurfaceInfo.Height; //一行なので高さは固定
				sr.rect.left = width*i; //左端
				sr.rect.right = width*(i + 1); //右端（実際の領域より+1して指定する）
				m_subRegions.push_back(sr);
			}
		}

		m_rect = SetRectFromWH(SurfaceInfo.Width, SurfaceInfo.Height);
	}
	pSurface->Release();


}
*/

unsigned Texture::GetWidth() const
{
	return m_width;
}

unsigned Texture::GetHeight() const
{
	return m_height;
}

LP_TEXTURE Texture::GetTexture() const
{
	return m_tex;
}


Rect Texture::GetSubRegion(const int n) const
{
	return m_subRegions.at(n).rect;
}

Texture& Texture::operator=(const Texture& tex)
{
	//自己代入をチェック
	if (this == &tex) return *this;

	//LP_TEXTUREの参照カウンタを足す
	m_tex = tex.m_tex;
	m_tex->AddRef();

	//スタックオブジェクトをそのままコピー
	this->m_texName = tex.m_texName;
	this->m_width = tex.m_width;
	this->m_height = tex.m_height;
	this->m_subRegions = tex.m_subRegions;

	return *this;
}

}