/*
アニメーション制御クラス
(前回のフレームからの経過時間を貰えばよい)
派生クラスで特殊なアニメーション（イーズインとか）
これをSubstance渡す
*/
#pragma once

#include<Timer.h>
#include<MyTypes.h>
#include<vector>

namespace Inferno
{
class Animation
{
public:
	enum TransitType
	{
		Unknown,
		Linear,
		EaseIn,
		EaseOut
	};

	struct KeyFrameSet
	{
		KeyFrameSet() { this->Clear(); }
		int key;
		Millisec delay;
		Millisec dur;
		TransitType type;
		bool loop;
		int start;
		int end;

		void Clear()
		{
			key = 0;
			delay = 0;
			dur = 0;
			type = Unknown;
			loop = false;
			start = 0;
			end = 0;
		}
	};

public:
	Animation();
	virtual ~Animation();

	//ディレイ時間、終了時間、開始点、到達点、ループ設定、（同期/非同期[待つか待たないか]）
	//到達座標のみ-1で現在座標
	void Set(const Millisec delay, const Millisec dur, const int s, const int e, const TransitType type, const bool loopFlag = false);
	void KF_Set(const int key,const Millisec delay, const Millisec dur,
		const int s, const int e, const TransitType type = Linear, const bool loopFlag = false);
	//設定と同時にスタート（主にテスト用）
	void Start(const Millisec delay, const Millisec dur, const int s, const int e, const TransitType type, const bool loopFlag = false);
	//登録済みのスタート
	void Start();
	int GetValue();
	bool HasEnded() const;
	//キーフレームを設定した場合、再び使う前にリセット
	void KF_Clear();

	static const TransitType InterpretTransitType(const std::wstring str);

private:
	Timer m_timer;
	std::vector<KeyFrameSet> m_kfset;
	int m_curKey;
	int m_endKey;
	Millisec m_endTime;
};



}