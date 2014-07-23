/*
Scripter.h
�X�N���v�g�G���W��
*/
#pragma once
#include<string>
#include<fstream>
#include<locale>
#include<codecvt>
#include<regex>
#include<string>
#include"Inferno\MyException.h"

namespace
{
	const int Maxbuff = 1024;
}

namespace Inferno
{
class Scripter
{
public:
	void LoadSceneFromScript(const std::wstring& filename)
	{
		//�Â����P�[�����ꎞ�ۑ�
		std::locale oldLocale;

		//�܂�ansi�ŊJ���ăG���R�[�h���`�F�b�N
		//�󂯕t����̂�sjis��utf8�B�f�t�H���g��utf8
		{
			std::ifstream ifpeek(filename);
			if (ifpeek.fail())
			{
				throw GeneralFileError(filename, L"�t�@�C�����J�����ƂɎ��s���܂���");
			}
			char buff[Maxbuff];
			ifpeek.getline(buff, 1024); //��s�����ǂ�

			//����ibom�t�����l���j
			std::regex patternU8("(\xEF\xBB\xBF)?encoding\\(UTF8\\)", std::regex_constants::extended);
			bool resultU8 = std::regex_match(buff, patternU8);
			if (resultU8) m_encoding = Encoding::e_utf8;

			std::regex patternSJIS("(\xEF\xBB\xBF)?encoding\\(SJIS\\)", std::regex_constants::extended);
			bool resultSJIS = std::regex_match(buff, patternSJIS);
			if (resultSJIS) m_encoding = Encoding::e_sjis;
		}

		//�󃍃P�[�� -> utf-8
		const std::locale empty_locale = std::locale::empty();
		typedef std::codecvt_utf8<char> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale(empty_locale, converter);

		std::ifstream in(filename);
		in.imbue(utf8_locale); //utf-8�ŊJ��
		if (in.fail())
		{
			throw GeneralFileError(filename, L"�t�@�C�����J�����ƂɎ��s���܂���");
		}
		
		char buff[1024];
		while (in.good())
		{
			in.getline(buff, 1024);
		}
		//while (in.good())
		//{
		//	//�ǂ̃G���R�[�f�B���O�œǂݍ��ނ�
		//	switch (m_encoding)
		//	{
		//	case Encoding::e_sjis:
		//		in.getline(buff, 1024);
		//		break;

		//	case Encoding::e_utf8:

		//		break;
		//	default:
		//		break;
		//	}

		//	//�G���R�[�h�̎w��
		//	in.imbue(locale("ja_JP.UTF-8"));
		//}
		//int stop = 0;

		//�Â����P�[����ǂݖ߂�
		std::locale::global(oldLocale);
	}

private:
	enum class Encoding
	{
		e_sjis,
		e_utf16,
		e_utf8,
	};

	Encoding m_encoding = Encoding::e_utf8;
};

}