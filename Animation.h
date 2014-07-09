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
	enum TransitType
	{
		Liner,
		EasyIn,
	};
public:
	Animation();
	virtual ~Animation();


	//ディレイ時間、終了時間、開始点、到達点、ループ設定、（同期/非同期[待つか待たないか]）
	//到達座標のみ-1で現在座標
	void Start(const Millisec delay, const Millisec dur, const int s, const int e, const bool loopFlag=false);
	int GetVaule();
	bool HasEnded() const;

private:
	Timer m_timer;
	Millisec m_delay;
	Millisec m_dur;
	TransitType m_type;
	bool m_loop;
	int m_start;
	int m_end;
};



}