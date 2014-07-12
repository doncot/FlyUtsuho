/*
GameCommon.h
ゲーム用の共通関数
*/
#pragma once
#include<cmath>

/*
点滅関数
timer：現在のカウント
base：最大上昇値
spd：スピード
*/
inline int Blink(int timer, int base, double spd) 
{
	return abs((int)(timer*spd) % base * 2 - base); 
}