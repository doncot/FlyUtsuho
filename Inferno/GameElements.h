/*
ゲーム要素は全てここに置く
Idea:イデア。本質を表す。以下のクラスは全てイデアを持つ
Substance:実在。「動ける」絵。基本形
Rigidbody:剛体。「物理衝突」をするSubstance。
全てのゲーム要素は自身の中心が原点座標になる
※Windows依存
*/
#pragma once
#include<Windows.h>
#include"Graphics.h"
#include<Texture.h>
#include"MyTypes.h"
#include"Common.h"
#include<Timer.h>

namespace Inferno
{
//ゲーム要素の状態（アクションゲーム用なので継承先に本来あるべき
enum GEAttribute
{
	UserLock = 1, //ユーザーからの操作をロック
	Draw = 2, //描画するか否か
	NoHit = 4, //衝突判定をするか否か
};

class Idea
{
public:
	Idea();
	~Idea();

	int GetWidth() const;
	int GetHeight() const;
	Vec2<int> GetDrawBase() const;
	//テクスチャの設定を渡すことを考えてのテクスチャクラス渡しだが
	///冗長なのでファイル名だけにした方がよい
	void SetTexture(const Texture& tex);
	const Texture* GetTexture() const;

private:
	const Texture* m_tex;

	Vec2<int> m_drawBase; //描画基準（ここでは絵の中心）
	int m_width;
	int m_height;

private:
	//コピーコンストラクタ＆代入演算子は不必要
	Idea(const Idea&) = delete;
	Idea& operator=(const Idea&) = delete;
};

//ゲームの個々要素
//二次元座標で動作すること前提
class Substance
{
public:
	Substance();
	Substance(const Idea& idea);
	virtual ~Substance();

	void SetIdea(const Idea& idea);

	//絶対座標指定移動
	virtual void AMove(const int x, const int y);
	//絶対座標指定移動
	virtual void AMove(const Vec2<int>& c);
	//相対標指定移動
	virtual void RMove(const int x, const int y);
	virtual void RMove(const Vec2<int>& c);
	//左上座標で位置を指定
	void SetPosofULCorner(const int x, const int y);
	//時計周りに回転（度数指定）
	void Rotate(const int degree);
	void Rotate(const float degree);

	Vec2<int> GetPosition() const;

	//位置座標とサイズから現在領域を求める
	Rect GetRegion() const;

	//属性（描画可能など）を指定
	void SetAttribute(GEAttribute attr, bool value);
	bool CheckAttribute(GEAttribute attr) const;
	
	void SetAlpha(const int alpha);

	void SetID(const int id);
	int GetID() const;

	//DispatchMessage(GMessageQueue)
	//InterpretMessage(GMessageQueue);

	virtual void Update();

	void Draw(const Graphics& g) const;

	//コピーコンストラクタ＆代入演算子必要
	Substance& operator=(const Substance& s);

protected:
	int m_id;
	const Idea* m_idea;
	Vec2<int> m_pos; //現在座標
	float m_angle; //度数法で回転情報
	int m_alpha;
	char m_attribute; //属性（描画属性など）を指定
};

//単数しかない要素の場合、いちいちイデアから実体を作るのが面倒なのでこっちを使う
//中身はイデアを内部に持つ実体である。
class Instant: public Substance
{
typedef Substance Base;
public:
	~Instant()
	{
		SAFE_DELETE(m_idea);
		SAFE_DELETE(m_tex);
	}

	void Initialize()
	{
		m_tex = new Texture();
		m_idea = new Idea();
		SetAttribute(GEAttribute::Draw, true);
	}
	void LoadTextureFromFile(const Graphics& g, const wstring& str)
	{
		m_tex->LoadImageFile(g, str);
		m_idea->SetTexture(*m_tex);
		Base::m_idea = m_idea;
	}

private:
	//自分で書き込む必要があるので、constなsubstaceのideaは使えない
	Idea* m_idea;
	Texture* m_tex;

};

}