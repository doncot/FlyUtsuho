/*
人工知能とは名ばかりの敵オブジェクト制御クラス
STG専用AIの予定だが使いまわせるかもしれないので、AIという名前でとりあえずは
*Animationのラッパーっぽい所が気になる
*/

#pragma once
#include"Inferno\MyTypes.h"
#include"Inferno\Timer.h"

namespace Inferno
{
	class AI
	{
	public:
		//直線移動を設定
		//void SetAnimation(const AnimationSet aSet);


		void Update(Vec2<int>* pos);

	private:

	};
}