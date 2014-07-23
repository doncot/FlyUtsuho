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
			std::regex patternU8("(\xEF\xBB\xBF)?encoding\\(UTF8\\)", std::regex_constants::extended);
			bool resultU8 = std::regex_match(buff, patternU8);
			if (resultU8) m_encoding = Encoding::e_utf8;

			std::regex patternSJIS("(\xEF\xBB\xBF)?encoding\\(SJIS\\)", std::regex_constants::extended);
			bool resultSJIS = std::regex_match(buff, patternSJIS);
			if (resultSJIS) m_encoding = Encoding::e_sjis;
		}

		//空ロケール -> utf-8
		const std::locale empty_locale = std::locale::empty();
		typedef std::codecvt_utf8<char> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale(empty_locale, converter);

		std::ifstream in(filename);
		in.imbue(utf8_locale); //utf-8で開く
		if (in.fail())
		{
			throw GeneralFileError(filename, L"ファイルを開くことに失敗しました");
		}
		
		char buff[1024];
		while (in.good())
		{
			in.getline(buff, 1024);
		}
		//while (in.good())
		//{
		//	//どのエンコーディングで読み込むか
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

		//	//エンコードの指定
		//	in.imbue(locale("ja_JP.UTF-8"));
		//}
		//int stop = 0;

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