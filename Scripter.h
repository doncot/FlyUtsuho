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
#include<cstdlib>
#include"Inferno\TString.h"
#include"ShooterScene.h"
#include"Inferno\MyException.h"
#include"Animation.h"

namespace
{
	const int Maxbuff = 1024;

	bool IsComment(const string& str)
	{
		std::regex pattern("^//.*",	std::regex_constants::extended);
		return std::regex_match(str, pattern);
	}

	bool IsComment(const wstring& str)
	{
		std::wregex pattern(L"^//.*", std::regex_constants::extended);
		return std::regex_match(str, pattern);
	}
}

namespace Inferno
{
class Scripter
{
public:
	static void LoadSceneFromScript(ShooterScene* scene, const std::wstring& filename)
	{
		//�g�p����G���R�[�f�B���O�i�f�t�H���g��utf8�j
		Encoding encoding = Encoding::e_utf8;
		//�Â����P�[�����ꎞ�ۑ�
		std::locale oldLocale;

		//�܂�ansi�ŊJ���ăG���R�[�h���`�F�b�N
		//�󂯕t����̂�sjis��utf8�B�f�t�H���g��utf8
		{
			std::ifstream ifpeek(filename);
			ifpeek.imbue(std::locale("En-US"));
			if (ifpeek.fail())
			{
				throw GeneralFileError(filename, L"�t�@�C�����J�����ƂɎ��s���܂���");
			}
			char buff[Maxbuff];
			ifpeek.getline(buff, 1024); //��s�����ǂ�

			//�G���R�[�f�B���O����ibom�t�����l���j
			//utf8
			std::regex patternU8("(\xEF\xBB\xBF)?encoding\\(UTF-?8\\)", 
				std::regex_constants::extended | std::regex_constants::icase);
			bool resultU8 = std::regex_match(buff, patternU8);
			if (resultU8) encoding = Encoding::e_utf8;
			//sjis
			std::regex patternSJIS("encoding\\(SJIS\\)", 
				std::regex_constants::extended | std::regex_constants::icase);
			bool resultSJIS = std::regex_match(buff, patternSJIS);
			if (resultSJIS) encoding = Encoding::e_sjis;
		}

		std::wifstream in;
		//�J���G���R�[�f�B���O��I��
		switch (encoding)
		{
		case Encoding::e_utf8:
			in.open(filename);
			//utf-8��utf16�ŊJ���iconsume_header=BOM����菜���j
			in.imbue(std::locale(in.getloc(), 
				new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, consume_header>));
			break;

		case Encoding::e_sjis:
			in.open(filename);
			in.imbue(std::locale("japanese"));
			break;

		default:
			throw GeneralFileError(filename, L"�G���R�[�h�ɕs��������܂�");
		}

		if (in.fail())
		{
			throw GeneralFileError(filename, L"�t�@�C�����J�����ƂɎ��s���܂���");
		}
		
		wchar_t buff[1024];
		match_results<const wchar_t*> match;
		while (in.good())
		{
			//��s�ǂݍ���
			in.getline(buff, 1024);

			//�R�����g�͓ǂݔ�΂�
			if (IsComment(buff)) continue;

			//encoding���߂͔�΂�
			{
				std::wregex pattern(L"encoding\\(.+\\)",
					std::regex_constants::extended | std::regex_constants::icase);
				if (std::regex_match(buff, pattern))
					continue;
			}

			//create����
			{
				std::wregex pattern(L"^create\\([[:s:]]*([[:d:]]+)[[:s:]]*\\)$", std::regex_constants::extended);
				if (std::regex_match(buff, match, pattern))
				{
					//id
					scene->CreateEnemy(std::atoi( TString(match.str(1)).GetStringA()));
					continue;
				}
			}

			//loadimage����
			{
				std::wregex pattern(L"^loadimage\\([[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*\"([^\"]+)\"[[:s:]]*\\)$",
					std::regex_constants::extended);
				if (std::regex_match(buff, match, pattern))
				{
					scene->SetImageFromFile(std::atoi( TString(match.str(1)).GetStringA() ),
						match.str(2));

					continue;
				}
			}

			//deploy����
			{
				std::wregex pattern(L"^deploy\\([[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*\\[[[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*([[:d:]]+)[[:s:]]*\\][[:s:]]*\\)$");
				if (std::regex_match(buff, match, pattern))
				{
					scene->RegisterDeploy(std::atoi(TString(match.str(1)).GetStringA()),
						std::atoi(TString(match.str(2)).GetStringA()),
						Vec2<int>(std::atoi(TString(match.str(3)).GetStringA()),
						std::atoi(TString(match.str(4)).GetStringA()))
						);
					continue;
				}
			}

			//move����
			std::wregex pattern(L"^move\\([[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*\\[[[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*([[:d:]]+)[[:s:]]*\\],([0-9]*\\.?[0-9]),([[:upper:]]+)[[:s:]]*\\)$");
			if (std::regex_match(buff, match, pattern))
			{
				scene->RegisterMove(std::atoi(TString(match.str(1)).GetStringA()),
					std::atoi(TString(match.str(2)).GetStringA()),
					Vec2<int>(std::atoi(TString(match.str(3)).GetStringA()),
						std::atoi(TString(match.str(4)).GetStringA())),
						std::atof(TString(match.str(5)).GetStringA()),
						Animation::InterpretTransitType(match.str(6))
					);
				
				continue;
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
};

}