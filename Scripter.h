/*
Scripter.h
�X�N���v�g�G���W��
*/
#pragma once
#include<string>
#include<fstream>
#include"Inferno\MyException.h"

namespace Inferno
{
class Scripter
{
public:
	void LoadSceneFromScript(const std::wstring& filename)
	{
		std::wifstream in(filename);
		if (in.fail())
		{
			throw GeneralFileError(filename, L"�t�@�C�����J�����ƂɎ��s���܂���");
		}

		wchar_t buff[1024];
		while (in.good())
		{
			in.getline(buff, 1024);
		}
		int stop = 0;
	}


};

}