#include"MyException.h"

namespace Inferno{

CreationFailed::CreationFailed(const std::string& cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message = cause + "の作成に失敗しました。";
}

CreationFailed::CreationFailed(const char* cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message = std::string(cause) + "の作成に失敗しました。";
}

CreationFailed::CreationFailed(const std::wstring& cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = cause + L"の作成に失敗しました。";
}

CreationFailed::CreationFailed(const wchar_t* cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = std::wstring(cause) + L"の作成に失敗しました。";
}

const char* CreationFailed::what() const
{
	return m_message.c_str();
}

const wchar_t* CreationFailed::what_w() const
{
	return m_message_w.c_str();
}

CreationFailed::~CreationFailed() {}

}