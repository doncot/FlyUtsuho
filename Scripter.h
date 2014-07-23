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

	bool IsComment(const string& str)
	{
		std::regex pattern("^//.*",	std::regex_constants::extended);
		return std::regex_match(str, pattern);
	}

	bool IsCreate(const string& str)
	{
		std::regex pattern("^create\\(\s*([^,]+)\s*,\s*([^)]+)\\)$", std::regex_constants::extended);
		return std::regex_match(str, pattern);
	}
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
			std::regex patternU8("(\xEF\xBB\xBF)?encoding\\(UTF8\\)", 
				std::regex_constants::extended | std::regex_constants::icase);
			bool resultU8 = std::regex_match(buff, patternU8);
			if (resultU8) m_encoding = Encoding::e_utf8;

			std::regex patternSJIS("encoding\\(SJIS\\)", 
				std::regex_constants::extended | std::regex_constants::icase);
			bool resultSJIS = std::regex_match(buff, patternSJIS);
			if (resultSJIS) m_encoding = Encoding::e_sjis;
		}

		//�󃍃P�[�� -> utf-8
		const std::locale empty_locale = std::locale::empty();
		typedef std::codecvt_utf8<char> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale(empty_locale, converter);

		std::ifstream in;
		//�J���G���R�[�f�B���O��I��
		switch (m_encoding)
		{
		case Encoding::e_utf8:
			in.open(filename);
			in.imbue(utf8_locale); //utf-8�ŊJ��
			break;

		case Encoding::e_sjis:
			//�f�t�H���g�̃��P�[���͓��{��Ȃ̂ł���ɂ܂�����i���ۉ����l����Ƃ܂������j
			in.open(filename);
			break;

		default:
			throw GeneralFileError(filename, L"�G���R�[�h�ɕs��������܂�");
		}

		if (in.fail())
		{
			throw GeneralFileError(filename, L"�t�@�C�����J�����ƂɎ��s���܂���");
		}
		
		char buff[1024];
		bool match = false;
		while (in.good())
		{
			//��s�ǂݍ���
			in.getline(buff, 1024);

			//�R�����g�͓ǂݔ�΂�
			if (IsComment(buff)) continue;

			if (IsCreate(buff))
			{
				int test = 2;
			}
		}


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