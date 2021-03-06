/*
オリジナルテクスチャ（をあれやコレする）クラス
※ライブラリ側からは実装非依存にする
*/
#pragma once
#include<d3d9.h> //こいつらは本当は追い出したい(実装隠蔽)
#include<d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include<string>
#include<vector>
#include"Graphics.h"
#include"TString.h"
#include"MyTypes.h"

namespace Inferno{
//テクスチャを内部的に区切るために使う
struct SubRegion
{
	int id; //ID。0を描画で指定すると全体を表示する
	Rect rect;
};

class Texture{
public:
	Texture();
	~Texture();

	//テクスチャを画像ファイルよりロード。分割はなし
	void LoadImageFile(const Graphics& g, const wstring& filename);

	unsigned GetWidth() const;
	unsigned GetHeight() const;
	LP_TEXTURE GetTexture() const;
	const wstring GetName() const;
	//Rect GetSubRegion(const int n) const;

	//LP_TEXTUREがあるから、deepcopyが必要
	Texture& operator=(const Texture& tex);

private:
	LP_TEXTURE m_tex; //LP_TEXTURE自体がItextureインターフェイスへのポインタ
	wstring m_name;
	unsigned m_width;	//結局これ使うんかい！！
	unsigned m_height; 
	//vector<SubRegion> m_subRegions; //テクスチャの中の分割したやつを記憶
};
}