#include"MyException.h"
#include<sstream>

namespace Inferno{

GeneralFileError::GeneralFileError(const std::string& filename, const std::string& message)
	: MyExceptionBase_RuntimeError("")
{
	m_message = "一般ファイルエラー：" + message + '(' + filename + ')';
}

GeneralFileError::GeneralFileError(const char* filename, const char* message)
	: MyExceptionBase_RuntimeError("")
{
	std::stringstream ss;
	ss << "一般ファイルエラー：" << message << '(' << filename << ')';
	m_message = ss.str();
}

GeneralFileError::GeneralFileError(const std::wstring& filename, const std::wstring& message)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = L"一般ファイルエラー：" + message + L'(' + filename + L')';
}

GeneralFileError::GeneralFileError(const wchar_t* filename, const wchar_t* message)
	: MyExceptionBase_RuntimeError("")
{
	std::wstringstream ss;
	ss << L"一般ファイルエラー：" << message << L'(' << filename << L')';
	m_message_w = ss.str();
}

const char* GeneralFileError::what() const
{
	return m_message.c_str();
}

const wchar_t* GeneralFileError::what_w() const
{
	return m_message_w.c_str();
}

GeneralFileError::~GeneralFileError() {}

}