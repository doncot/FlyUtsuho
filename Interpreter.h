/*
Scripter.h
スクリプトエンジン
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

	//識別子
	//const wchar_t* const ID = 
	const wstring SPACES = L"[[:s:]]*";
	const wstring STRING = L"\"([^\"]*)\"";
}

namespace Inferno
{
class Scripter
{
public:
	static void LoadSceneFromScript(ShooterScene* scene, const std::wstring& filename)
	{
		//使用するエンコーディング（デフォルトはutf8）
		Encoding encoding = Encoding::e_utf8;
		//古いロケールを一時保存
		std::locale oldLocale;

		//まずansiで開いてエンコードをチェック
		//受け付けるのはsjisかutf8。デフォルトはutf8
		{
			std::ifstream ifpeek(filename);
			ifpeek.imbue(std::locale("En-US"));
			if (ifpeek.fail())
			{
				throw GeneralFileError(filename, L"ファイルを開くことに失敗しました");
			}
			char buff[Maxbuff];
			ifpeek.getline(buff, 1024); //一行だけ読む

			//エンコーディング判定（bom付きも考慮）
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
		//開くエンコーディングを選択
		switch (encoding)
		{
		case Encoding::e_utf8:
			in.open(filename);
			//utf-8をutf16で開く（consume_header=BOMを取り除く）
			in.imbue(std::locale(in.getloc(), 
				new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, consume_header>));
			break;

		case Encoding::e_sjis:
			in.open(filename);
			in.imbue(std::locale("japanese"));
			break;

		default:
			throw GeneralFileError(filename, L"エンコードに不備があります");
		}

		if (in.fail())
		{
			throw GeneralFileError(filename, L"ファイルを開くことに失敗しました");
		}
		
		wchar_t buff[1024];
		match_results<const wchar_t*> match;
		while (in.good())
		{
			//一行読み込む
			in.getline(buff, 1024);

			//コメントは読み飛ばす
			if (IsComment(buff)) continue;

			//encoding命令は飛ばす
			{
				std::wregex pattern(L"encoding\\(.+\\)",
					std::regex_constants::extended | std::regex_constants::icase);
				if (std::regex_match(buff, pattern))
					continue;
			}

			//loadresource命令
			{
				std::wregex pattern(L"^[[:s:]]*([[:alnum:]_]+)[[:s:]]*=[[:s:]]*loadresource\\([[:s:]]*\"([^\"]+)\"[[:s:]]*\\)$",
					std::regex_constants::extended);
				if (std::regex_match(buff, match, pattern))
				{
					scene->CreateEnemyResourceFromFile(match.str(1),
						match.str(2));

					continue;
				}
			}

			
			//Instance.create命令
			{
				std::wregex pattern(L"^[[:s:]]*([[:alnum:]_]+)[[:s:]]*=[[:s:]]*([[:alnum:]]+)\\.create\\(\\)$", std::regex_constants::extended);
				if (std::regex_match(buff, match, pattern))
				{
					//id
					scene->CreateEnemyInstance(match.str(2), match.str(1));
					continue;
				}
			}
			

			//deploy命令
			{
				std::wregex pattern(L"^([[:alnum:]_]+)\\.deploy\\([[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*\\[[[:s:]]*(-?[[:d:]]+)[[:s:]]*,[[:s:]]*(-?[[:d:]]+)[[:s:]]*\\][[:s:]]*\\)$");
				if (std::regex_match(buff, match, pattern))
				{
					scene->RegisterDeploy(match.str(1),
						std::atoi(TString(match.str(2)).GetStringA()),
						Vec2<int>(std::atoi(TString(match.str(3)).GetStringA()),
						std::atoi(TString(match.str(4)).GetStringA()))
						);
					continue;
				}
			}

			//move命令
			{
				std::wregex pattern(L"^([[:alnum:]_]+)\\.move\\([[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*\\[[[:s:]]*(-?[[:d:]]+)[[:s:]]*,[[:s:]]*(-?[[:d:]]+)[[:s:]]*\\][[:s:]]*,[[:s:]]*\\[[[:s:]]*(-?[[:d:]]+)[[:s:]]*,[[:s:]]*(-?[[:d:]]+)[[:s:]]*\\][[:s:]]*,[[:s:]]*([[:upper:]]+)[[:s:]]*\\)$");
				if (std::regex_match(buff, match, pattern))
				{
					scene->RegisterMove( match.str(1),
						std::atoi(TString(match.str(2)).GetStringA()),
						std::atoi(TString(match.str(3)).GetStringA()),
						Vec2<int>(std::atoi(TString(match.str(4)).GetStringA()),
						std::atoi(TString(match.str(5)).GetStringA())),
						Vec2<int>(std::atoi(TString(match.str(6)).GetStringA()),
						std::atoi(TString(match.str(7)).GetStringA())),
						Animation::InterpretTransitType(match.str(8))
						);
					continue;
				}
			}

			//stepmove命令
			{
				std::wregex pattern(L"^stepmove\\([[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*\\[[[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*([[:d:]]+)[[:s:]]*\\],([0-9]*\\.?[0-9]),([[:upper:]]+)[[:s:]]*\\)$");
				if (std::regex_match(buff, match, pattern))
				{
					/*
					scene->RegisterMove(std::atoi(TString(match.str(1)).GetStringA()),
						std::atoi(TString(match.str(2)).GetStringA()),
						Vec2<int>(std::atoi(TString(match.str(3)).GetStringA()),
						std::atoi(TString(match.str(4)).GetStringA())),
						std::atof(TString(match.str(5)).GetStringA()),
						Animation::InterpretTransitType(match.str(6))
						);
					continue;
					*/
				}
			}

			//ここまで来て読めない物があったらエラーを出す
		}

		//古いロケールを読み戻す
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