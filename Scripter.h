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
	static void LoadSceneFromScript(ShooterScene* scene, const Graphics& g, const std::wstring& filename)
	{
		//仕様するエンコーディング
		Encoding encoding = Encoding::e_utf8;
		//古いロケールを一時保存
		std::locale oldLocale;

		//まずansiで開いてエンコードをチェック
		//受け付けるのはsjisかutf8。デフォルトはutf8
		{
			std::ifstream ifpeek(filename);
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
			//utf-8をutf16で開く
			in.imbue(std::locale(in.getloc(), new std::codecvt_utf8_utf16<wchar_t>));
			break;

		case Encoding::e_sjis:
			//デフォルトのロケールは日本語なのでそれにまかせる（国際化を考えるとまずいが）
			in.open(filename);
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
				std::wregex pattern(L"(\xEF\xBB\xBF)?encoding\\(.+\\)",
					std::regex_constants::extended | std::regex_constants::icase);
				if (std::regex_match(buff, pattern)) continue;
			}

			//create命令
			{
				std::wregex pattern(L"^create\\([[:s:]]*([^)]+)[[:s:]]*\\)$", std::regex_constants::extended);
				if (std::regex_match(buff, match, pattern))
				{
					//id
					scene->CreateEnemy(std::atoi( TString(match.str(1)).GetStringA()));
					continue;
				}
			}

			//loadimage命令
			{
				std::wregex pattern(L"^loadimage\\([[:s:]]*([[:d:]]+)[[:s:]]*,[[:s:]]*\"([^\"]+)\"[[:s:]]*\\)$",
					std::regex_constants::extended);
				if (std::regex_match(buff, match, pattern))
				{
					Texture newTex;
					//TString使用は応急処置
					newTex.LoadImageFile(g,match.str(2));
					scene->SetTexture(std::atoi( TString(match.str(1)).GetStringA() ), newTex);
				}
			}
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