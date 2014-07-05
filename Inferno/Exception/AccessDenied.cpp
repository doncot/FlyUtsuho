#include"MyException.h"

namespace Inferno{

AccessDenied::AccessDenied(const std::string& cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message = cause + "へのアクセスができませんでした。";
}

AccessDenied::AccessDenied(const char* cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message = std::string(cause) + "へのアクセスができませんでした。";
}

AccessDenied::AccessDenied(const std::wstring& cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = cause + L"へのアクセスができませんでした。";
}

AccessDenied::AccessDenied(const wchar_t* cause)
	: MyExceptionBase_RuntimeError("")
{
	m_message_w = std::wstring(cause) + L"へのアクセスができませんでした。";
}

const char* AccessDenied::what() const
{
	return m_message.c_str();
}

const wchar_t* AccessDenied::what_w() const
{
	return m_message_w.c_str();
}

AccessDenied::~AccessDenied() {}

}