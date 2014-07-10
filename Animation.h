/*
アニメーション制御クラス
(前回のフレームからの経過時間を貰えばよい)
派生クラスで特殊なアニメーション（イーズインとか）
これをSubstance渡す
*/
#include<Timer.h>
#include<MyTypes.h>
#include<vector>

namespace Inferno
{
class Animation
{
public:
	//TODOスペル確認
	enum TransitType
	{
		TT_Linear,
		TT_EaseIn,
		TT_EaseOut
	};

	Animation();
	virtual ~Animation();


	//ディレイ時間、終了時間、開始点、到達点、ループ設定、（同期/非同期[待つか待たないか]）
	//到達座標のみ-1で現在座標
	void Start(const Millisec delay, const Millisec dur, const int s, const int e, const TransitType type, const bool loopFlag=false);
	void KF_Set(const int key,const Millisec delay, const Millisec dur,
		const int s, const int e, const TransitType type = TT_Linear, const bool loopFlag = false);
	int GetValue();
	bool HasEnded() const;

private:
	struct KeyFrameSet
	{
		KeyFrameSet() :key(0), delay(0), dur(0), loop(false), start(0), end(0){}
		int key;
		Millisec delay;
		Millisec dur;
		TransitType type;
		bool loop;
		int start;
		int end;
	};

	Timer m_timer;
	std::vector<KeyFrameSet> m_kfset;
	int m_curKey;
	int m_endKey;
	Millisec m_endTime;
};



}