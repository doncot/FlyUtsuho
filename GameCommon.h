/*
GameCommon.h
�Q�[���p�̋��ʊ֐�
*/
#pragma once
#include<cmath>

namespace Inferno
{

	/*
	�_�Ŋ֐�
	timer�F���݂̃J�E���g
	base�F�ő�㏸�l
	spd�F�X�s�[�h
	*/
	inline int Blink(int timer, int base, double spd)
	{
		return abs((int)(timer*spd) % base * 2 - base);
	}


}