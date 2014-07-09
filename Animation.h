/*
アニメーション制御クラス
(前回のフレームからの経過時間を貰えばよい)
派生クラスで特殊なアニメーション（イーズインとか）
これをSubstance渡す
*/
#include<Timer.h>
#include<MyTypes.h>

namespace Inferno
{
	//substance側からアニメの種類を設定
	enum Animetype
	{
		Transform,
	};

class Animation
{
	//TODOスペル確認
	enum Transittype
	{
		Liner,
		EasyIn,
	};
public:
	Animation();
	virtual ~Animation();


	//ディレイ時間、終了時間、開始座標、到達座標、ループ設定、（同期/非同期[待つか待たないか]）
	//到達座標のみ-1で現在座標
	void SetTransform(const Millisec delay, const Millisec dur, const Vec2<int> s, const Vec2<int> e, const bool loopFlag=false);

	//透明度を変化させる
	////ディレイ時間、終了時間、開始透明度、到達透明度、ループ設定、（同期/非同期[待つか待たないか]）
	void SetAlpha(Millisec delay, Millisec dur, float s, float e);

	Vec2<int> DoTransform(const Millisec frameTime) const;
	float DoAlpha(const Millisec frameTime);

	//substanceにオーバーライドしてもらう
	//後で純粋にする
	virtual void Update(){};
	
private:
	Timer m_timer;
	Millisec m_delay;
	Millisec m_dur;
	Animetype m_type;
	//transform
	Vec2<int> m_startCor;
	Vec2<int> m_endCor;
	//rotate
	//scale
	//alpha
	float m_startAlpha;
	float m_endAlpha;
	bool m_loop;
};



}