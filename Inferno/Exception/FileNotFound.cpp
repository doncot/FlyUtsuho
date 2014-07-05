#include"MyException.h"

namespace Inferno{

FileNotFound::FileNotFound(const std::string& filename)
	: MyExceptionBase_RuntimeError("")
{
	m_message = "エラー：ファイル\"" + filename + "\"がありません。";
}

FileNotFound::FileNotFound(const char* filename)
	: MyExceptionBase_RuntimeError("")
{
	m_message = "エラー：ファイル\"" + std::string(filename) + "\"がありません。";
}

FileNotFound::FileNotFound(const std::wstring& filename)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = L"エラー：ファイル\"" + filename + L"\"がありません。";
}

FileNotFound::FileNotFound(const wchar_t* filename)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = L"エラー：ファイル\"" + std::wstring(filename) + L"\"がありません。";
}

const char* FileNotFound::what() const
{
	return m_message.c_str();
}

const wchar_t* FileNotFound::what_w() const
{
	return m_message_w.c_str();
}

FileNotFound::~FileNotFound() {}

}