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

			//判定（bom付きも考慮）
			std::regex patternU8("(\xEF\xBB\xBF)?encoding\\(UTF8\\)", 
				std::regex_constants::extended | std::regex_constants::icase);
			bool resultU8 = std::regex_match(buff, patternU8);
			if (resultU8) m_encoding = Encoding::e_utf8;

			std::regex patternSJIS("encoding\\(SJIS\\)", 
				std::regex_constants::extended | std::regex_constants::icase);
			bool resultSJIS = std::regex_match(buff, patternSJIS);
			if (resultSJIS) m_encoding = Encoding::e_sjis;
		}

		//空ロケール -> utf-8
		const std::locale empty_locale = std::locale::empty();
		typedef std::codecvt_utf8<char> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale(empty_locale, converter);

		std::ifstream in;
		//開くエンコーディングを選択
		switch (m_encoding)
		{
		case Encoding::e_utf8:
			in.open(filename);
			in.imbue(utf8_locale); //utf-8で開く
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
		
		char buff[1024];
		bool match = false;
		while (in.good())
		{
			//一行読み込む
			in.getline(buff, 1024);

			//コメントは読み飛ばす
			if (IsComment(buff)) continue;

			if (IsCreate(buff))
			{
				int test = 2;
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

	Encoding m_encoding = Encoding::e_utf8;
};

}